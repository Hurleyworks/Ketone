// Copyright 2008-present Contributors to the OpenImageIO project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/OpenImageIO/oiio/blob/master/LICENSE.md


/// \file
/// Declarations for things that are used privately by ImageIO.


#ifndef OPENIMAGEIO_IMAGEIO_PVT_H
#define OPENIMAGEIO_IMAGEIO_PVT_H

#include <OpenImageIO/imageio.h>
#include <OpenImageIO/thread.h>
#include <OpenImageIO/timer.h>



OIIO_NAMESPACE_BEGIN

namespace pvt {

/// Mutex allowing thread safety of ImageOutput internals
///
extern recursive_mutex imageio_mutex;
extern atomic_int oiio_threads;
extern atomic_int oiio_read_chunk;
extern ustring plugin_searchpath;
extern std::string format_list;
extern std::string input_format_list;
extern std::string output_format_list;
extern std::string extension_list;
extern std::string library_list;
extern int oiio_print_debug;
extern int oiio_log_times;


// For internal use - use error() below for a nicer interface.
void seterror (string_view message);

/// Use error() privately only.  Protoype is conceptually printf-like, but
/// also fully typesafe:
template<typename... Args>
inline void errorf (const char* fmt, const Args&... args) {
    seterror(Strutil::sprintf (fmt, args...));
}

// Make sure all plugins are inventoried.  Should only be called while
// imageio_mutex is held.  For internal use only.
void catalog_all_plugins (std::string searchpath);

/// Given the format, set the default quantization range.
void get_default_quantize (TypeDesc format, long long &quant_min,
                           long long &quant_max) noexcept;

/// Turn potentially non-contiguous-stride data (e.g. "RGBxRGBx") into
/// contiguous-stride ("RGBRGB"), for any format or stride values
/// (measured in bytes).  Caller must pass in a dst pointing to enough
/// memory to hold the contiguous rectangle.  Return a ptr to where the
/// contiguous data ended up, which is either dst or src (if the strides
/// indicated that data were already contiguous).
const void *contiguize (const void *src, int nchannels,
                        stride_t xstride, stride_t ystride, stride_t zstride, 
                        void *dst, int width, int height, int depth,
                        TypeDesc format);

/// Turn contiguous data from any format into float data.  Return a
/// pointer to the converted data (which may still point to src if no
/// conversion was necessary).
const float *convert_to_float (const void *src, float *dst, int nvals,
                               TypeDesc format);

/// Turn contiguous float data into any format.  Return a pointer to the
/// converted data (which may still point to src if no conversion was
/// necessary).
const void *convert_from_float (const float *src, void *dst,
                                size_t nvals, TypeDesc format);

/// A version of convert_from_float that will break up big jobs with
/// multiple threads.
const void *parallel_convert_from_float (const float *src, void *dst,
                                         size_t nvals, TypeDesc format);

/// Internal utility: Error checking on the spec -- if it contains texture-
/// specific metadata but there are clues it's not actually a texture file
/// written by maketx or `oiiotool -otex`, then assume these metadata are
/// wrong and delete them. Return true if we think it's one of these
/// incorrect files and it was fixed.
OIIO_API bool check_texture_metadata_sanity (ImageSpec &spec);

/// Internal function to log time recorded by an OIIO::timer(). It will only
/// trigger a read of the time if the "log_times" attribute is set or the
/// OPENIMAGEIO_LOG_TIMES env variable is set.
OIIO_API void log_time (string_view key, const Timer& timer);

/// Get the timing report from log_time entries.
OIIO_API std::string timing_report ();

/// An object that, if oiio_log_times is nonzero, logs time until its
/// destruction. If oiio_log_times is 0, it does nothing.
class LoggedTimer {
public:
    LoggedTimer (string_view name) : m_timer(oiio_log_times) {
        if (oiio_log_times)
            m_name = name;
    }
    ~LoggedTimer () {
        if (oiio_log_times)
            log_time (m_name, m_timer);
    }
    void stop () { m_timer.stop(); }
    void start () { m_timer.start(); }
    void rename (string_view name) { m_name = name; }
private:
    Timer m_timer;
    std::string m_name;
};

}  // namespace pvt

OIIO_NAMESPACE_END

//Define a default plugin search path
#define OIIO_DEFAULT_PLUGIN_SEARCHPATH "@PLUGIN_SEARCH_PATH_NATIVE@"

#endif // OPENIMAGEIO_IMAGEIO_PVT_H
