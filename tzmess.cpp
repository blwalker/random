#include <ctime>
#include <cstring>
#include <cstdio>
#include <cstdlib>

time_t convertToUtc(const char* dtStr, const char* tzStr)
{
	printf("%s%s\n", dtStr, tzStr);
	struct tm dt;
	struct tm tz;
	memset(&dt, 0, sizeof(dt));
	memset(&tz, 0, sizeof(tz));

	strptime(dtStr, "%F %T", &dt);
	strptime(tzStr, "%z", &tz);

	dt.tm_isdst = -1;
	return timegm(&dt) - tz.tm_gmtoff;
}

void convertToTz(const time_t utc, const char* tz, struct tm& tm)
{
	const char* tzEnv = getenv("TZ");

	memset(&tm, 0, sizeof(tm));

	setenv("TZ", tz, 1);
	tzset();
	localtime_r(&utc, &tm);

	if (tzEnv)
		setenv("TZ", tzEnv, 1);
	else
		unsetenv("TZ");

	tzset();

	static char buf[128];
	memset(buf, 0, sizeof(buf));

	strftime(buf, sizeof(buf), "%F %T %z", &tm);
	printf("%s %s\n", buf, tz);
}

int main(int argc, char* argv[])
{
	struct tm tm;

	time_t utc = convertToUtc("2018-08-14 12:00:00.0", "-0400");
	convertToTz(utc, "America/New_York", tm);
	convertToTz(utc, "America/Chicago", tm);
	convertToTz(utc, "America/Denver", tm);
	convertToTz(utc, "America/Phoenix", tm);
	convertToTz(utc, "America/Los_Angeles", tm);
	convertToTz(utc, "Pacific/Honolulu", tm);

	printf("\n");
	utc = convertToUtc("2018-02-02 12:00:00.0", "-0800");
	convertToTz(utc, "America/New_York", tm);
	convertToTz(utc, "America/Chicago", tm);
	convertToTz(utc, "America/Denver", tm);
	convertToTz(utc, "America/Phoenix", tm);
	convertToTz(utc, "America/Los_Angeles", tm);
	convertToTz(utc, "Pacific/Honolulu", tm);
	return 0;
}

