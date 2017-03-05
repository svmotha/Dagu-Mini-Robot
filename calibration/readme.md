# DAGU Mini Driver Project | Calibration

Calibration is essential for equipment that must respond to external stimuli. All sensor calibration is done to account for any sensor error (statistical or systematic), hysteresis, and other environmental factors that may affect our sensors not working to spec.  

## Contents
- [Data Set Building](#data-set-building)
- [Trend Identification](#trend-identification)

#### Data Set Building
All calibration processes take place through a fundamentally iterative methodology. Multiple readings are taken and stored into an array. The array size is directly proportional to the accuracy of the calibration process i.e. the larger the amount of readings collected, the larger the accuracy of the calibration process. The current calibration process takes 15 readings. This is purely a subjective amount, it has no prehistoric empirical data to support its effects on the optimization of sensor calibration processes.

#### Trend Identification
Each data set is collected with essentially one parameter gradually changing to ensure accuracy can be measured. This (depending on the type of sensor) is done incrementally. For the Infrared sensor this, for example, involved taking each set of 15 readings from an incrementally changing distance, starting from about 20.0 cm from our wall sample.

These reading sets are summarized into fundamentally six values, before being logged, tabulated, and graphed. Namely these are:

1. The actual distance e.g. 20.0 cm.
2. The detected distance mean i.e. all 15 values averaged out, e.g. 20.43 cm.
3. The systematic error i.e. difference between actual and detected distance mean, which in this case happens to be 0.43 cm.
4. The minimum value found within the data set, e.g. 19.87 cm.
5. The maximum value found within the data set, e.g. 21.43 cm.
6. The statistical error i.e. the difference between the minimum and maximum values found above, which in this case would be 1.56 cm.

All the mean values for the various incremental readings are then graphed for trendline identification. Using Microsoft EXCEL or its OS counterpart Numbers, one can generate an exponential trendline i.e. in the form:
```
x = (23.694 / y)^(1 / 0.7478)
```


#### Error Identification