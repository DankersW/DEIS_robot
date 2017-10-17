"""LCM type definitions
This file automatically generated by lcm.
DO NOT MODIFY BY HAND!!!!
"""

try:
    import cStringIO.StringIO as BytesIO
except ImportError:
    from io import BytesIO
import struct

class line_sensors_t(object):
    __slots__ = ["left", "middle", "right"]

    def __init__(self):
        self.left = 0
        self.middle = 0
        self.right = 0

    def encode(self):
        buf = BytesIO()
        buf.write(line_sensors_t._get_packed_fingerprint())
        self._encode_one(buf)
        return buf.getvalue()

    def _encode_one(self, buf):
        buf.write(struct.pack(">hhh", self.left, self.middle, self.right))

    def decode(data):
        if hasattr(data, 'read'):
            buf = data
        else:
            buf = BytesIO(data)
        if buf.read(8) != line_sensors_t._get_packed_fingerprint():
            raise ValueError("Decode error")
        return line_sensors_t._decode_one(buf)
    decode = staticmethod(decode)

    def _decode_one(buf):
        self = line_sensors_t()
        self.left, self.middle, self.right = struct.unpack(">hhh", buf.read(6))
        return self
    _decode_one = staticmethod(_decode_one)

    _hash = None
    def _get_hash_recursive(parents):
        if line_sensors_t in parents: return 0
        tmphash = (0x7f486316df7eb9fa) & 0xffffffffffffffff
        tmphash  = (((tmphash<<1)&0xffffffffffffffff)  + (tmphash>>63)) & 0xffffffffffffffff
        return tmphash
    _get_hash_recursive = staticmethod(_get_hash_recursive)
    _packed_fingerprint = None

    def _get_packed_fingerprint():
        if line_sensors_t._packed_fingerprint is None:
            line_sensors_t._packed_fingerprint = struct.pack(">Q", line_sensors_t._get_hash_recursive([]))
        return line_sensors_t._packed_fingerprint
    _get_packed_fingerprint = staticmethod(_get_packed_fingerprint)
