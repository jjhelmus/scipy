
import functools
import numpy as np
import scipy.ndimage as ndimage

def assert_close(x, y):
    assert np.allclose(x, y)

# footprints / structures
cross = np.array([[0, 1, 0],
                  [1, 1, 1],
                  [0, 1, 0]])
square = np.ones((3, 3))
square2 = np.ones((2,2))

# dilation related setup
im_d = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
                 [0, 0, 1, 1, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 1, 0, 0],
                 [0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
                 [0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
                 [0, 0, 0, 0, 1, 1, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]], dtype=np.uint8)
grey_dilation = ndimage.grey_dilation
cross_dilate = ndimage.binary_dilation(im_d).view(np.uint8)
square_dilate = ndimage.binary_dilation(im_d, structure=square).view(np.uint8)
square2_dilate = ndimage.binary_dilation(im_d, structure=square2).view(np.uint8)
# erosion related setup
im_e = square_dilate
grey_erosion = functools.partial(ndimage.grey_erosion, mode='constant')
cross_erode = ndimage.binary_erosion(im_e).view(np.uint8)
square_erode = ndimage.binary_erosion(im_e, structure=square).view(np.uint8)
square2_erode = ndimage.binary_erosion(im_e, structure=square2).view(np.uint8)

def test_dilation_cross_footprint():
    assert_close(grey_dilation(im_d, footprint=cross), cross_dilate)
def test_dilation_cross_structure():
    assert_close(grey_dilation(im_d, structure=cross), cross_dilate+1)

def test_dilation_square_footprint():
    assert_close(grey_dilation(im_d, footprint=square), square_dilate)
def test_dilation_square_structure():
    assert_close(grey_dilation(im_d, structure=square), square_dilate+1)
def test_dilation_scalar_size():
    assert_close(grey_dilation(im_d, size=3), square_dilate)
def test_dilation_size_tuple():
    assert_close(grey_dilation(im_d, size=(3, 3)), square_dilate)
                   
# check origin adjustment for structures with even numbered sides
def test_dilation_origin_shift_footprint():
    assert_close(grey_dilation(im_d, footprint=square2), square2_dilate)
def test_dilation_origin_shift_structure():
    assert_close(grey_dilation(im_d, structure=square2), square2_dilate+1)
def test_dilation_origin_shift_scalar_size():
    assert_close(grey_dilation(im_d, size=2), square2_dilate)
def test_dilation_origin_shift_size_tuple():
    assert_close(grey_dilation(im_d, size=(2, 2)), square2_dilate)

def test_erosion_cross_footprint():
    assert_close(grey_erosion(im_e, footprint=cross), cross_erode)
def test_erosion_cross_structure():
    assert_close(grey_erosion(im_e+1, structure=cross), cross_erode)
                   
def test_erosion_square_footprint():
    assert_close(grey_erosion(im_e, footprint=square), square_erode)
def test_erosion_square_structure():
    assert_close(grey_erosion(im_e+1, structure=square, cval=1), square_erode)
def test_erosion_scalar_size():
    assert_close(grey_erosion(im_e, size=3), square_erode)
def test_erosion_size_tuple():
    assert_close(grey_erosion(im_e, size=(3, 3)), square_erode)
                   
# check origin adjustment for structures with even numbered sides
def test_erosion_origin_shift_footprint():
    assert_close(grey_erosion(im_e, footprint=square2), square2_erode)
def test_erosion_origin_shift_structure():
    assert_close(grey_erosion(im_e+1, structure=square2, cval=1), square2_erode)
def test_erosion_origin_shift_scalar_size():
    assert_close(grey_erosion(im_e, size=2), square2_erode)
def test_erosion_origin_shift_size_tuple():
    assert_close(grey_erosion(im_e, size=(2, 2)), square2_erode)

if __name__ == '__main__':
    import nose
    nose.runmodule()