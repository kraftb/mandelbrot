#!/usr/bin/php
<?php

$DEBUG = ($argv[1] === 'DEBUG') ? true : false;

$frames = 2000;
// $frames = 200;
// $target = 0.000001;
$target = 0.00000001;

$zoom_x = -0.6875;
$zoom_y = 0.3385416664;


$start_x_min = -2;
$start_x_max = 2;

$start_y_min = -2;
$start_y_max = 2;


$end_x_min = $zoom_x - $target;
$end_x_max = $zoom_x + $target;

$end_y_min = $zoom_y - $target;
$end_y_max = $zoom_y + $target;

if (strpos($argv[1], 'PIXEL:') === 0) {
    $pixel = substr($argv[1], strlen('PIXEL:'));
    list($x, $y) = explode(',', $pixel);
    echo $x . "\n";
    echo $y . "\n";
    exit();
}


/**
 * Return a value ranging from $start to $end.
 *
 * Scale the value so that the steps become
 * smaller as $index counts up to $maxIndex.
 *
 * @param integer $index: Runs from 0 to $maxIndex - 1
 * @param integer $maxIndex: The max index
 * @param float $start: The value which should be returned for $index==0
 * @param float $end: The value which should be returned for $index==$maxIndex-1
 * @return float
 */
function scale_linear($index, $maxIndex, $start, $end) {
	// Use a linear function: y = k * x + d

	// $start = k * $index:"0" + $start
	$d = $start;

	// x = index
	$x = $index;

	$d_y = $end - $start;
	$d_x = ($maxIndex - 1) - 0;

	$k = $d_y / $d_x;

	$y = $k * $x + $d;

	return $y;
}

/**
 * Return a value ranging from $start to $end.
 *
 * Scale the value so that the steps become
 * smaller as $index counts up to $maxIndex.
 *
 * @param integer $index: Runs from 0 to $maxIndex - 1
 * @param integer $maxIndex: The max index
 * @param float $start: The value which should be returned for $index==0
 * @param float $end: The value which should be returned for $index==$maxIndex-1
 * @return float
 */
function scale_quadratic($index, $maxIndex, $start, $end) {
	// Use a quadratic function: y = a*x² + b*x + c

	// Normalize x (0 .. 1):
	$x = $index / ($maxIndex-1);

	// We want to have large steps (d_y) in the beginning
	// (for small $index) and small steps at the end.
	// If we visualise ourselves a quadratic function then
	// the d_y values become larger for larger x. So we
	// need the x = 1-x instead:
	$x = 1 - $x;

	$c = $end;
	$b = 0;
	$a = ($start - $end);

	$y = $a * $x*$x + $b*$x + $c;
	return $y;
}

/**
 * Return a value ranging from $start to $end.
 *
 * Scale the value so that the steps become
 * smaller as $index counts up to $maxIndex.
 *
 * @param integer $index: Runs from 0 to $maxIndex - 1
 * @param integer $maxIndex: The max index
 * @param float $start: The value which should be returned for $index==0
 * @param float $end: The value which should be returned for $index==$maxIndex-1
 * @param float $expRange: The range in the exponential function which to utilize
 * @return float
 */
function scale_logarythmic($index, $maxIndex, $start, $end, $expRange) {
	// Use a logarythmic function: y = a*e^(b*x) + c

	// Normalize x (0 .. 1):
	$x = $index / ($maxIndex-1);

	// We want to have large steps (d_y) in the beginning
	// (for small $index) and small steps at the end.
	// For an exponential/logarythmic function the
	// the d_y values become larger for larger x.
    // So we need the x = 1-x instead.
    // Additionall we need to accomodate for the fact that
    // e^0 = 1. The complete exp() function has an offset of 1.
	$x = 1 - $x;

//	$c = $end - 1;
	$c = $end;
	$b = $expRange;
	$a = ($start - $end);

//       Multiply by range
//       |    Normalized exponential function
//       |    |                              Offset
//       |    |                              |
	$y = $a * ((exp($b*$x)-1)/(exp($b)-1)) + $c;
	return $y;
}

/**
 * Return a value ranging from $start to $end.
 *
 * Scale the value so that the steps become
 * smaller as $index counts up to $maxIndex.
 *
 * @param integer $index: Runs from 0 to $maxIndex - 1
 * @param integer $maxIndex: The max index
 * @param float $start: The value which should be returned for $index==0
 * @param float $end: The value which should be returned for $index==$maxIndex-1
 * @return float
 */
function scale_geometric($index, $maxIndex, $start, $end) {
}


/**
 * Return a value ranging from $start to $end.
 *
 * Scale the value so that the steps become
 * smaller as $index counts up to $maxIndex.
 *
 * @param integer $index: Runs from 0 to $maxIndex - 1
 * @param integer $maxIndex: The max index
 * @param float $start: The value which should be returned for $index==0
 * @param float $end: The value which should be returned for $index==$maxIndex-1
 * @return float
 */
function scale($index, $maxIndex, $start, $end) {
	// Linear
	// Looks too quick in the end
//  return scale_linear($index, $maxIndex, $start, $end);
//  return scale_quadratic($index, $maxIndex, $start, $end);
    return scale_logarythmic($index, $maxIndex, $start, $end, 10);
//  return scale_geometric($index, $maxIndex, $start, $end);
}

/**
 * Debug the scaling functions
 *
 * @param integer $index: Runs from 0 to $maxIndex - 1
 * @param integer $maxIndex: The max index
 * @param float $start: The value which should be returned for $index==0
 * @param float $end: The value which should be returned for $index==$maxIndex-1
 * @return void
 */
function debug_scale($index, $maxIndex, $start, $end) {
    $l = scale_linear($index, $maxIndex, $start, $end);
    $q = scale_quadratic($index, $maxIndex, $start, $end);
    $e1 = scale_logarythmic($index, $maxIndex, $start, $end, 1);
    $e5 = scale_logarythmic($index, $maxIndex, $start, $end, 5);
    $e10 = scale_logarythmic($index, $maxIndex, $start, $end, 10);
    $g = scale_geometric($index, $maxIndex, $start, $end);
    $out = $index . ';' . $l . ';' . $q . ';' . $e1 . ';' . $e5 . ';' . $e10 . ';' . $g . "\n";
    $out = str_replace('.', ',', $out);
    echo $out;
}

for ($frame = 0; $frame < $frames; $frame++) {

    if ($DEBUG) {
        debug_scale($frame, $frames, 0, 1);
    } else {
        $x_min = scale($frame, $frames, $start_x_min, $end_x_min);
        $x_max = scale($frame, $frames, $start_x_max, $end_x_max);
        $y_min = scale($frame, $frames, $start_y_min, $end_y_min);
        $y_max = scale($frame, $frames, $start_y_max, $end_y_max);

        $file = 'image_' . sprintf("%05d", $frame) . '.png';
        echo $frame . ' ' . $file . ' ' . $x_min . ' ' . $y_min . ' ' . $x_max . ' ' . $y_max . "\n";
    }
}

