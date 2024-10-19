#include "Rm3100.h"

Rm3100::Rm3100(TwoWire& wire, uint8_t addr) : _wire(wire), _addr(addr), _scale(1.0f, 1.0f, 1.0f), _sample_delay_ms(0) {
    // Constructor implementation
}

Rm3100::~Rm3100() {
    // Destructor implementation
}

int Rm3100::Begin() {
    uint8_t rev;
    int ret = GetHardwareRevision(&rev);
    if (ret != RM3100_RET_OK) {
        return ret;
    }
    if (rev != RM3100_REVID) {
        return RM3100_RET_ENODEV;
    }
    
    // Set default cycle counts and update measurement scale
    struct CycleCounts cc = { 200, 200, 200 };
    SetCycleCounts(&cc);
    
    return RM3100_RET_OK;
}

int Rm3100::GetHardwareRevision(uint8_t *rev) {
    return Read(RM3100_REG_REVID, rev, 1);
}

int Rm3100::SetCycleCounts(struct CycleCounts *cc) {
    uint8_t buffer[6];
    buffer[0] = (cc->x >> 8) & 0xFF;
    buffer[1] = cc->x & 0xFF;
    buffer[2] = (cc->y >> 8) & 0xFF;
    buffer[3] = cc->y & 0xFF;
    buffer[4] = (cc->z >> 8) & 0xFF;
    buffer[5] = cc->z & 0xFF;
    
    int ret = Write(RM3100_REG_CCX, buffer, 6);
    if (ret == RM3100_RET_OK) {
        UpdateMeasurementScale(cc);
    }
    return ret;
}

int Rm3100::SetCycleCounts(uint16_t xyz) {
    struct CycleCounts cc = { xyz, xyz, xyz };
    return SetCycleCounts(&cc);
}

int Rm3100::SetRate(float rate) {
    uint8_t tmrc;
    if (rate >= 600) tmrc = RM3100_CMM_RATE_600_0_HZ;
    else if (rate >= 300) tmrc = RM3100_CMM_RATE_300_0_HZ;
    else if (rate >= 150) tmrc = RM3100_CMM_RATE_150_0_HZ;
    else if (rate >= 75) tmrc = RM3100_CMM_RATE_75_0_HZ;
    else if (rate >= 37) tmrc = RM3100_CMM_RATE_37_0_HZ;
    else if (rate >= 18) tmrc = RM3100_CMM_RATE_18_0_HZ;
    else if (rate >= 9) tmrc = RM3100_CMM_RATE_9_0_HZ;
    else if (rate >= 4.5) tmrc = RM3100_CMM_RATE_4_5_HZ;
    else if (rate >= 2.3) tmrc = RM3100_CMM_RATE_2_3_HZ;
    else if (rate >= 1.2) tmrc = RM3100_CMM_RATE_1_2_HZ;
    else if (rate >= 0.6) tmrc = RM3100_CMM_RATE_0_6_HZ;
    else if (rate >= 0.3) tmrc = RM3100_CMM_RATE_0_3_HZ;
    else if (rate >= 0.015) tmrc = RM3100_CMM_RATE_0_015_HZ;
    else tmrc = RM3100_CMM_RATE_0_075_HZ;
    
    return SetContinuousMeasurementModeUpdateRate(tmrc);
}

int Rm3100::SetContinuousMeasurementMode(bool enabled) {
    struct ContinuousMeasurementMode cmm = { 0 };
    cmm.start = enabled ? 1 : 0;
    cmm.cmx = 1;
    cmm.cmy = 1;
    cmm.cmz = 1;
    cmm.drdm = RM3100_DRDM_ALL_AXES;
    
    uint8_t buffer = *((uint8_t*)&cmm);
    return Write(RM3100_REG_CMM, &buffer, 1);
}

int Rm3100::GetStatus(struct Status *status) {
    uint8_t buffer;
    int ret = Read(RM3100_REG_STATUS, &buffer, 1);
    if (ret == RM3100_RET_OK) {
        *status = *((struct Status*)&buffer);
    }
    return ret;
}

int Rm3100::GetSample(struct Sample *s) {
    struct Measurement m;
    int ret = GetMeasurement(&m);
    if (ret == RM3100_RET_OK) {
        s->x = m.x * _scale.x;
        s->y = m.y * _scale.y;
        s->z = m.z * _scale.z;
    }
    return ret;
}

int Rm3100::GetMeasurement(struct Measurement *m) {
    uint8_t buffer[9];
    int ret = Read(RM3100_REG_MX, buffer, 9);
    if (ret == RM3100_RET_OK) {
        m->x = (int32_t)((buffer[0] << 16) | (buffer[1] << 8) | buffer[2]) >> 8;
        m->y = (int32_t)((buffer[3] << 16) | (buffer[4] << 8) | buffer[5]) >> 8;
        m->z = (int32_t)((buffer[6] << 16) | (buffer[7] << 8) | buffer[8]) >> 8;
    }
    return ret;
}

void Rm3100::UpdateMeasurementScale(struct CycleCounts *cc) {
    _scale.x = 0.3671 * (float)cc->x;
    _scale.y = 0.3671 * (float)cc->y;
    _scale.z = 0.3671 * (float)cc->z;
}

int Rm3100::SetContinuousMeasurementModeUpdateRate(uint8_t tmrc) {
    uint8_t buffer = tmrc | RM3100_CMM_RATE_MSB;
    return Write(RM3100_REG_TMRC, &buffer, 1);
}

int Rm3100::Read(uint8_t reg, uint8_t *buffer, uint8_t count) {
    _wire.beginTransmission(_addr);
    _wire.write(reg);
    if (_wire.endTransmission(false) != 0) {
        return RM3100_RET_EIO;
    }
    
    _wire.requestFrom(_addr, count);
    if (_wire.available() != count) {
        return RM3100_RET_EIO;
    }
    
    for (uint8_t i = 0; i < count; i++) {
        buffer[i] = _wire.read();
    }
    
    return RM3100_RET_OK;
}

int Rm3100::Write(uint8_t reg, uint8_t *buffer, uint8_t count) {
    _wire.beginTransmission(_addr);
    _wire.write(reg);
    _wire.write(buffer, count);
    if (_wire.endTransmission() != 0) {
        return RM3100_RET_EIO;
    }
    return RM3100_RET_OK;
}