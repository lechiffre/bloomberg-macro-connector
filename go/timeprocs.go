package blpconngo

import (
	"time"
)

// MaxValidMicroseconds represents the maximum valid timestamp
// (year 3000-01-01 00:00:00 UTC = 32503680000000000 microseconds)
// Anything larger is likely an "unset" or invalid value
const MaxValidMicroseconds uint64 = 32503680000000000

// IsValidDateTime checks if a microseconds value represents a valid datetime
// Returns false for unset/invalid values (very large numbers close to uint64 max)
func IsValidDateTime(microseconds uint64) bool {
	return microseconds < MaxValidMicroseconds
}

func ToNativeTime(microseconds uint64, offset int16) time.Time {
	seconds := int64(microseconds / 1e6)
	nanoseconds := int64((microseconds % 1e6) * 1e3)
	location := time.FixedZone("UTC", int(offset)*60)
	return time.Unix(seconds, nanoseconds).In(location)
}



