//
// Created by Borchers, Henry Samuel on 12/6/16.
//

#include "filedialog.h"
#include "noc_file_dialog.h"

#define FILTER_VIDEO_FILES "Video Files\0*.mov;*.mp4;*.mpg;*.mkv;*.avi\0"

const char *getFilename() {
    return noc_file_dialog_open(NOC_FILE_DIALOG_OPEN, FILTER_VIDEO_FILES, NULL, NULL);

}
