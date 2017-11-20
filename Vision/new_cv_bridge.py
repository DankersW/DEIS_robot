
import cv2
import image_encodings
class CvBridgeError(TypeError):
    """
    This is the error raised by :class:`cv_bridge.CvBridge` methods when they fail.
    """
    pass

class CvBridge:
    def __init__(self):
        self.cvtype_to_name = {}

        for t in ["8U", "8S", "16U", "16S", "32S", "32F", "64F" ]:
            for c in [1,2,3,4]:
                nm = "%sC%d" % (t, c)
                self.cvtype_to_name[eval("cv2.CV_%s" % nm)] = nm

        self.numpy_type_to_cvtype = {'uint8':'8U', 'int8':'8S', 'uint16':'16U',
                                        'int16':'16S', 'int32':'32S', 'float32':'32F',
                                        'float64':'64F'}
        self.numpy_type_to_cvtype.update(dict((v, k) for (k, v) in self.numpy_type_to_cvtype.items()))
    def imgmsg_to_cv2(self, img_msg, desired_encoding="passthrough"):
        """
        Convert a sensor_msgs::Image message to an OpenCV :ctype:`cv::Mat`.
        :param img_msg:   A sensor_msgs::Image message
        :param desired_encoding:  The encoding of the image data, one of the following strings:
            * ``"passthrough"``
            * one of the standard strings in sensor_msgs/image_encodings.h
        :rtype: :ctype:`cv::Mat`
        :raises CvBridgeError: when conversion is not possible.
        If desired_encoding is ``"passthrough"``, then the returned image has the same format as img_msg.
        Otherwise desired_encoding must be one of the standard image encodings
        This function returns an OpenCV :ctype:`cv::Mat` message on success, or raises :exc:`cv_bridge.CvBridgeError` on failure.
        """
        #import cv2
        import numpy as np
        dtype, n_channels = self.encoding_to_dtype_with_channels(img_msg.encoding)
        im = np.ndarray(shape=(img_msg.height, img_msg.width, n_channels),
                            dtype=dtype, buffer=img_msg.data)

        if desired_encoding == "passthrough":
            return im

        #from cv_bridge.boost.cv_bridge_boost import cvtColor2

        #try:
        #    res = cvtColor2(im, img_msg.encoding, desired_encoding)
        #except RuntimeError as e:
        #    raise CvBridgeError(e)

        #return res

    def encoding_to_dtype_with_channels(self, encoding):
        return self.cvtype2_to_dtype_with_channels(self.encoding_to_cvtype2(encoding))

    def cvtype2_to_dtype_with_channels(self, cvtype):
        import re
        vals = re.split('(.+)C(.+)', self.cvtype_to_name[cvtype])
        return self.numpy_type_to_cvtype[vals[1]], eval(vals[2])

    def encoding_to_cvtype2(self, encoding):
        if (encoding == image_encodings.BGR8):
            return 16
        #if (encoding == image_encodings.MONO8)  return CV_8UC1
        #if (encoding == image_encodings.RGB8)   return CV_8UC3
        #if (encoding == image_encodings.MONO16) return CV_16UC1
        #if (encoding == image_encodings.BGR16)  return CV_16UC3
        #if (encoding == image_encodings.RGB16)  return CV_16UC3
        #if (encoding == image_encodings.BGRA8)  return CV_8UC4
        #if (encoding == image_encodings.RGBA8)  return CV_8UC4
        #if (encoding == image_encodings.BGRA16) return CV_16UC4
        #if (encoding == image_encodings.RGBA16) return CV_16UC4

        """
int getCvType(const std::string& encoding)
{
  // Check for the most common encodings first
  if (encoding == enc::BGR8)   return CV_8UC3;
  if (encoding == enc::MONO8)  return CV_8UC1;
  if (encoding == enc::RGB8)   return CV_8UC3;
  if (encoding == enc::MONO16) return CV_16UC1;
  if (encoding == enc::BGR16)  return CV_16UC3;
  if (encoding == enc::RGB16)  return CV_16UC3;
  if (encoding == enc::BGRA8)  return CV_8UC4;
  if (encoding == enc::RGBA8)  return CV_8UC4;
  if (encoding == enc::BGRA16) return CV_16UC4;
  if (encoding == enc::RGBA16) return CV_16UC4;

  // For bayer, return one-channel
  if (encoding == enc::BAYER_RGGB8) return CV_8UC1;
  if (encoding == enc::BAYER_BGGR8) return CV_8UC1;
  if (encoding == enc::BAYER_GBRG8) return CV_8UC1;
  if (encoding == enc::BAYER_GRBG8) return CV_8UC1;
  if (encoding == enc::BAYER_RGGB16) return CV_16UC1;
  if (encoding == enc::BAYER_BGGR16) return CV_16UC1;
  if (encoding == enc::BAYER_GBRG16) return CV_16UC1;
if (encoding == enc::BAYER_GRBG16) return CV_16UC1;
        """