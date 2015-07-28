
#include "dp-bus.h"
#include "log.h"

int dpm_connect_service(void)
{
	int rc;
	dp_bus_connection *conn;

	conn = dp_bus_connect(DP_BUS_ADDRESS);
	if( conn == NULL)
		ERROR("Failed to connection dp-bus");

	return 0;
}
