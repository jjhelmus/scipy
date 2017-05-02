#!/usr/bin/env python
from __future__ import division, print_function, absolute_import

from os.path import join


def configuration(parent_package='', top_path=None):
    import warnings
    from numpy.distutils.misc_util import Configuration
    from numpy.distutils.system_info import get_info, BlasNotFoundError
    config = Configuration('odr', parent_package, top_path)

    libodr_files = ['d_odr.c',
                    'd_mprec.c',
                    'dlunoc.c']

    blas_info = get_info('blas_opt')
    if blas_info:
        libodr_files.append('d_lpk.c')
    else:
        warnings.warn(BlasNotFoundError.__doc__)
        libodr_files.append('d_lpkbls.c')

    odrpack_src = [join('odrpack', x) for x in libodr_files]
    config.add_library('odrpack', sources=odrpack_src)

    sources = ['__odrpack.c']
    libraries = ['odrpack'] + blas_info.pop('libraries', []) + ['f2c']
    include_dirs = ['.'] + blas_info.pop('include_dirs', [])
    blas_info['library_dirs'].append('/home/jjhelmus')
    config.add_extension('__odrpack',
        sources=sources,
        libraries=libraries,
        include_dirs=include_dirs,
        depends=(['odrpack.h'] + odrpack_src),
        **blas_info
    )

    config.add_data_dir('tests')
    return config

if __name__ == '__main__':
    from numpy.distutils.core import setup
    setup(**configuration(top_path='').todict())
