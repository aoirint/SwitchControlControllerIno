#pragma once

#include <HID.h>

class NoReportIDHID_ : public HID_
{
  public:
    NoReportIDHID_();
    int SendReport(const void *data, int len);
};

extern NoReportIDHID_ NoReportIDHID;
