#include "NoReportIDHID.h"

NoReportIDHID_::NoReportIDHID_()
{
}

int NoReportIDHID_::SendReport(const void *data, int len)
{
	auto ret = USB_Send(pluggedEndpoint | TRANSFER_RELEASE, data, len);
	return ret;
}

NoReportIDHID_ NoReportIDHID;
