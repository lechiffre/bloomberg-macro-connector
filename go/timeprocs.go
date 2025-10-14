package blpconngo

import (
	"time"
)

func ToNativeTime(microseconds uint64, offset int16) time.Time {
	seconds := int64(microseconds / 1e6)
	nanoseconds := int64((microseconds % 1e6) * 1e3)
	location := time.FixedZone("UTC", int(offset)*60)
	return time.Unix(seconds, nanoseconds).In(location)
}


