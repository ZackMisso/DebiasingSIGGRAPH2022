Image Editing Library

A small library for programmatically editing images and generating procedural
content. This library is specifically designed to support normal images as well
as 3D textures.

This library is separated into 5 different files:

image.h - the base image implementation along with reading and writing interfaces
filter.h - this file contains implementations of filters which can be applied to
           images.
im_util.h - this file contains a collection of miscelaneous and potentially useful
            methods.
noise.h - a simple perlin noise and turbulence implementation taken from PBRT
procedural.h - a collection of methods to generate procedurally generated textures
               which act as an interface to noise.h

The following sections will act as a summary of all methods within each file in
order that they were mentioned above.

Image.h

Global functions:

T im_clamp(T val, T min, T max)
    Input: the minimum, maximum, and value to be clamped
    Output: the clampled value
    Description: returns the value if the value lies within the minimum and
                 maximum bounds, otherwise returns the minimum or maximum
                 depending on if the value is either greater than the minimum,
                 or larger than the maximum, respectively.

unsigned char valToByte(T val)
    Input: a floating point value to be converted
    Output: an integer [0, 255]
    Description: this method converts a floating point representation of a pixel's
                 intensity to its byte form. Used for writing to file formats such
                 as png where the color channel's are stored as bytes. [0.0, 1.0]
                 is assumed to map to [0, 255]

void byteToVal(unsigned char in, T& val)
    Input: a byte representation of a pixel channel and a reference to the result
    Output: None
    Description: this method converts a byte representation of a pixel channel into
                 a floating point value ([0, 255] -> [0.0, 1.0])

std::string getExtension(std::string filename)
    Input: this method takes in as input a path to an image
    Output: this method outputs its extension as an std::string
    Description: self explanatory

Pixel struct:
    This struct just contains the r, g, and b components of a pixel. This struct
    is used mostly to simplify modifiers and accessors, and is not used for storing
    data.

Image class:
    The image's pixel intensities are all stored in a 1D std::vector. Images are
    stored in a col - major order.

    Constructors:
        Image()
            Initializes the image to be of size 0x0x0
        Image(int w, int h, int d)
            Initializes the image to be of size wxhxd
            Note: none of the pixel intensities are initialized
        Image(std::string filename)
            Reads and initializes the image from the provided path

    void clear()
        Description: sets all pixel intensities to 0

    double average()
        Description: returns the average of all pixel intensities

    Pixel average_pixel()
        Description: returns the average pixel intensity for each channel
                     individually.

    T max()
        Description: returns the max pixel intensity of all channels

    T min()
        Description: returns the min pixel intensity of all channels

    Pixel max_channel():
        returns the maximum intensity of each channel individually

    Pixel min_channel():
        returns the minimum intensity of each channel individually


Filter.h

Im_Util.h

Noise.h

Procedural.h
