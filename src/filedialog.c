//
// Created by Borchers, Henry Samuel on 12/6/16.
//

#include "filedialog.h"
#include "noc/noc_file_dialog.h"
const char *getFilename() {
#ifdef __APPLE__
    return noc_file_dialog_open(NOC_FILE_DIALOG_OPEN, "Quicktime\0*.mov\0Matroska\0*.mkv\0MPEG\0*.mpg\0MPEG-4\0*.mp4\0", NULL, NULL);
#else
    return noc_file_dialog_open(NOC_FILE_DIALOG_OPEN, "Video Files\0*.mov;*.mp4;*.mpg;*.mkv\0All Files\0*.*\0", NULL, NULL);
#endif


}
