"""LCM type definitions
This file automatically generated by lcm.
DO NOT MODIFY BY HAND!!!!
"""

try:
    import cStringIO.StringIO as BytesIO
except ImportError:
    from io import BytesIO
import struct

import exlcm.encoder_values_t

class sensor_vals_t(object):
    __slots__ = ["timestamp", "wheel_encoders"]

    def __init__(self):
        self.timestamp = 0
        self.wheel_encoders = exlcm.encoder_values_t()

    def encode(self):
        buf = BytesIO()
        buf.write(sensor_vals_t._get_packed_fingerprint())
        self._encode_one(buf)
        return buf.getvalue()

    def _encode_one(self, buf):
        buf.write(struct.pack(">q", self.timestamp))
        assert self.wheel_encoders._get_packed_fingerprint() == exlcm.encoder_values_t._get_packed_fingerprint()
        self.wheel_encoders._encode_one(buf)

    def decode(data):
        if hasattr(data, 'read'):
            buf = data
        else:
            buf = BytesIO(data)
        if buf.read(8) != sensor_vals_t._get_packed_fingerprint():
            raise ValueError("Decode error")
        return sensor_vals_t._decode_one(buf)
    decode = staticmethod(decode)

    def _decode_one(buf):
        self = sensor_vals_t()
        self.timestamp = struct.unpack(">q", buf.read(8))[0]
        self.wheel_encoders = exlcm.encoder_values_t._decode_one(buf)
        return self
    _decode_one = staticmethod(_decode_one)

    _hash = None
    def _get_hash_recursive(parents):
        if sensor_vals_t in parents: return 0
        newparents = parents + [sensor_vals_t]
        tmphash = (0x7a8550fd2f1bd89b+ exlcm.encoder_values_t._get_hash_recursive(newparents)) & 0xffffffffffffffff
        tmphash  = (((tmphash<<1)&0xffffffffffffffff)  + (tmphash>>63)) & 0xffffffffffffffff
        return tmphash
    _get_hash_recursive = staticmethod(_get_hash_recursive)
    _packed_fingerprint = None

    def _get_packed_fingerprint():
        if sensor_vals_t._packed_fingerprint is None:
            sensor_vals_t._packed_fingerprint = struct.pack(">Q", sensor_vals_t._get_hash_recursive([]))
        return sensor_vals_t._packed_fingerprint
    _get_packed_fingerprint = staticmethod(_get_packed_fingerprint)

