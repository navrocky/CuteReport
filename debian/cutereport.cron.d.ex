#
# Regular cron jobs for the cutereport package
#
0 4	* * *	root	[ -x /usr/bin/cutereport_maintenance ] && /usr/bin/cutereport_maintenance
