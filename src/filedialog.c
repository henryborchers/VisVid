//
// Created by Borchers, Henry Samuel on 12/6/16.
//

#include "filedialog.h"
#include "noc/noc_file_dialog.h"

#ifdef __APPLE__
#define FILTER_VIDEO_FILES "Quicktime\0*.mov\0Matroska\0*.mkv\0MPEG\0*.mpg\0MPEG-4\0*.mp4\0"
#else
#define FILTER_VIDEO_FILES "Video Files\\0*.mov;*.mp4;*.mpg;*.mkv\\0All Files\\0*.*\\0"
#endif

const char *getFilename() {
    return noc_file_dialog_open(NOC_FILE_DIALOG_OPEN, FILTER_VIDEO_FILES, NULL, NULL);

}
