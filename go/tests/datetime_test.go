package main

import (
	"testing"
	"time"
	"blpconngo"
)

// TestToNativeTimeBasic tests basic datetime conversion
func TestToNativeTimeBasic(t *testing.T) {
	// Unix epoch: 1970-01-01 00:00:00 UTC
	microseconds := uint64(0)
	offset := int16(0)
	result := blpconngo.ToNativeTime(microseconds, offset)
	
	expected := time.Unix(0, 0).UTC()
	if !result.Equal(expected) {
		t.Errorf("Expected %v, got %v", expected, result)
	}
}

// TestToNativeTimeWithMicroseconds tests conversion with microseconds
func TestToNativeTimeWithMicroseconds(t *testing.T) {
	// 2000-01-02 03:04:05.123456 UTC
	// January 2, 2000 at 03:04:05 = 946782245 seconds since epoch
	seconds := int64(946782245)
	microseconds := uint64(123456)
	totalMicros := uint64(seconds)*1e6 + microseconds
	offset := int16(0)
	
	result := blpconngo.ToNativeTime(totalMicros, offset)
	
	expected := time.Unix(seconds, int64(microseconds*1000)).UTC()
	if !result.Equal(expected) {
		t.Errorf("Expected %v, got %v", expected, result)
	}
	
	// Check individual components
	if result.Year() != 2000 {
		t.Errorf("Expected year 2000, got %d", result.Year())
	}
	if result.Month() != time.January {
		t.Errorf("Expected month January, got %s", result.Month())
	}
	if result.Day() != 2 {
		t.Errorf("Expected day 2, got %d", result.Day())
	}
	if result.Hour() != 3 {
		t.Errorf("Expected hour 3, got %d", result.Hour())
	}
	if result.Minute() != 4 {
		t.Errorf("Expected minute 4, got %d", result.Minute())
	}
	if result.Second() != 5 {
		t.Errorf("Expected second 5, got %d", result.Second())
	}
	if result.Nanosecond() != 123456000 {
		t.Errorf("Expected nanosecond 123456000, got %d", result.Nanosecond())
	}
}

// TestToNativeTimeWithOffset tests conversion with timezone offset
func TestToNativeTimeWithOffset(t *testing.T) {
	// 2023-10-15 14:30:45.123456 UTC+2 (offset = 120 minutes)
	// October 15, 2023 at 14:30:45 = 1697380245 seconds since epoch
	seconds := int64(1697380245)
	microseconds := uint64(123456)
	totalMicros := uint64(seconds)*1e6 + microseconds
	offset := int16(120) // UTC+2
	
	result := blpconngo.ToNativeTime(totalMicros, offset)
	
	// The time should be in UTC+2 timezone
	_, resultOffset := result.Zone()
	expectedOffset := int(offset) * 60 // Convert minutes to seconds
	if resultOffset != expectedOffset {
		t.Errorf("Expected offset %d seconds, got %d seconds", expectedOffset, resultOffset)
	}
	
	// The underlying Unix time should be the same
	if result.Unix() != seconds {
		t.Errorf("Expected Unix time %d, got %d", seconds, result.Unix())
	}
}

// TestToNativeTimeLeapYear tests conversion on a leap year date
func TestToNativeTimeLeapYear(t *testing.T) {
	// 2016-02-29 12:00:00 UTC (leap year)
	// February 29, 2016 at 12:00:00 = 1456747200 seconds since epoch
	seconds := int64(1456747200)
	totalMicros := uint64(seconds) * 1e6
	offset := int16(0)
	
	result := blpconngo.ToNativeTime(totalMicros, offset)
	
	if result.Year() != 2016 {
		t.Errorf("Expected year 2016, got %d", result.Year())
	}
	if result.Month() != time.February {
		t.Errorf("Expected month February, got %s", result.Month())
	}
	if result.Day() != 29 {
		t.Errorf("Expected day 29, got %d", result.Day())
	}
	if result.Hour() != 12 {
		t.Errorf("Expected hour 12, got %d", result.Hour())
	}
}

// TestToNativeTimeNegativeOffset tests conversion with negative timezone offset
func TestToNativeTimeNegativeOffset(t *testing.T) {
	// Test with UTC-5 (offset = -300 minutes)
	seconds := int64(1609459200) // 2021-01-01 00:00:00 UTC
	totalMicros := uint64(seconds) * 1e6
	offset := int16(-300) // UTC-5
	
	result := blpconngo.ToNativeTime(totalMicros, offset)
	
	// The time should be in UTC-5 timezone
	_, resultOffset := result.Zone()
	expectedOffset := int(offset) * 60 // Convert minutes to seconds
	if resultOffset != expectedOffset {
		t.Errorf("Expected offset %d seconds, got %d seconds", expectedOffset, resultOffset)
	}
}

// TestToNativeTimeMillisecondPrecision tests that microsecond precision is preserved
func TestToNativeTimeMicrosecondPrecision(t *testing.T) {
	// Test various microsecond values
	testCases := []struct {
		name   string
		micros uint64
	}{
		{"1 microsecond", 1},
		{"999 microseconds", 999},
		{"1000 microseconds", 1000},
		{"500000 microseconds", 500000},
		{"999999 microseconds", 999999},
	}
	
	baseSeconds := int64(1609459200) // 2021-01-01 00:00:00 UTC
	
	for _, tc := range testCases {
		t.Run(tc.name, func(t *testing.T) {
			totalMicros := uint64(baseSeconds)*1e6 + tc.micros
			result := blpconngo.ToNativeTime(totalMicros, 0)
			
			expectedNanos := int64(tc.micros * 1000)
			if result.Nanosecond() != int(expectedNanos) {
				t.Errorf("Expected nanosecond %d, got %d", expectedNanos, result.Nanosecond())
			}
		})
	}
}

// TestDateTimeRoundTrip tests that we can convert back and forth
func TestDateTimeRoundTrip(t *testing.T) {
	// Create a time
	original := time.Date(2023, time.December, 25, 15, 30, 45, 123456789, time.UTC)
	
	// Convert to microseconds
	microseconds := uint64(original.Unix())*1e6 + uint64(original.Nanosecond()/1000)
	
	// Convert back
	result := blpconngo.ToNativeTime(microseconds, 0)
	
	// Note: We lose sub-microsecond precision
	expectedNanos := (original.Nanosecond() / 1000) * 1000
	if result.Unix() != original.Unix() {
		t.Errorf("Unix time mismatch: expected %d, got %d", original.Unix(), result.Unix())
	}
	if result.Nanosecond() != expectedNanos {
		t.Errorf("Nanosecond mismatch: expected %d, got %d", expectedNanos, result.Nanosecond())
	}
}

// TestEdgeCases tests edge cases for datetime conversion
func TestDateTimeEdgeCases(t *testing.T) {
	testCases := []struct {
		name        string
		microseconds uint64
		offset      int16
		expectYear  int
		expectMonth time.Month
		expectDay   int
	}{
		{
			name:        "Unix epoch",
			microseconds: 0,
			offset:      0,
			expectYear:  1970,
			expectMonth: time.January,
			expectDay:   1,
		},
		{
			name:        "Y2K",
			microseconds: 946684800000000, // 2000-01-01 00:00:00 UTC
			offset:      0,
			expectYear:  2000,
			expectMonth: time.January,
			expectDay:   1,
		},
		{
			name:        "Leap second day 2012",
			microseconds: 1341100800000000, // 2012-07-01 00:00:00 UTC
			offset:      0,
			expectYear:  2012,
			expectMonth: time.July,
			expectDay:   1,
		},
	}
	
	for _, tc := range testCases {
		t.Run(tc.name, func(t *testing.T) {
			result := blpconngo.ToNativeTime(tc.microseconds, tc.offset)
			
			if result.Year() != tc.expectYear {
				t.Errorf("Expected year %d, got %d", tc.expectYear, result.Year())
			}
			if result.Month() != tc.expectMonth {
				t.Errorf("Expected month %s, got %s", tc.expectMonth, result.Month())
			}
			if result.Day() != tc.expectDay {
				t.Errorf("Expected day %d, got %d", tc.expectDay, result.Day())
			}
		})
	}
}
