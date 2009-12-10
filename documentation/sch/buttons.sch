v 20081231 1
C 40000 40000 0 0 0 title-B.sym
T 49500 40900 9 10 1 0 0 0 2
Buttons

C 52500 41900 1 0 0 ATmega16L-1.sym
{
T 55500 50500 5 10 1 1 0 6 1
refdes=PDIP40
T 52900 50650 5 10 0 0 0 0 1
device=ATmega16L_DIP
T 52900 50850 5 10 0 0 0 0 1
footprint=DIP40
T 52900 52050 5 10 0 0 0 0 1
symversion=2.0
}
C 45400 47100 1 90 0 resistor-2.sym
{
T 45050 47500 5 10 0 0 90 0 1
device=RESISTOR
T 45100 47300 5 10 1 1 90 0 1
refdes=1k
}
C 45400 43300 1 90 0 resistor-2.sym
{
T 45050 43700 5 10 0 0 90 0 1
device=RESISTOR
T 45100 43500 5 10 1 1 90 0 1
refdes=16k
}
C 45300 45500 1 90 0 switch-pushbutton-no-1.sym
{
T 45000 45900 5 10 1 1 90 0 1
refdes=up
T 44700 45900 5 10 0 0 90 0 1
device=SWITCH_PUSHBUTTON_NO
}
N 45300 46500 45300 47100 4
N 45300 44200 45300 45500 4
C 46400 47100 1 90 0 resistor-2.sym
{
T 46100 47300 5 10 1 1 90 0 1
refdes=1k
T 46050 47500 5 10 0 0 90 0 1
device=RESISTOR
}
C 46400 43300 1 90 0 resistor-2.sym
{
T 46100 43500 5 10 1 1 90 0 1
refdes=16k
T 46050 43700 5 10 0 0 90 0 1
device=RESISTOR
}
C 46300 45500 1 90 0 switch-pushbutton-no-1.sym
{
T 46000 45900 5 10 1 1 90 0 1
refdes=down
T 45700 45900 5 10 0 0 90 0 1
device=SWITCH_PUSHBUTTON_NO
}
N 46300 46500 46300 47100 4
N 46300 44200 46300 45500 4
C 47400 47100 1 90 0 resistor-2.sym
{
T 47100 47300 5 10 1 1 90 0 1
refdes=1k
T 47050 47500 5 10 0 0 90 0 1
device=RESISTOR
}
C 47400 43300 1 90 0 resistor-2.sym
{
T 47100 43500 5 10 1 1 90 0 1
refdes=16k
T 47050 43700 5 10 0 0 90 0 1
device=RESISTOR
}
C 47300 45500 1 90 0 switch-pushbutton-no-1.sym
{
T 47000 45900 5 10 1 1 90 0 1
refdes=change0
T 46700 45900 5 10 0 0 90 0 1
device=SWITCH_PUSHBUTTON_NO
}
N 47300 46500 47300 47100 4
N 47300 44200 47300 45500 4
C 48400 47100 1 90 0 resistor-2.sym
{
T 48100 47300 5 10 1 1 90 0 1
refdes=1k
T 48050 47500 5 10 0 0 90 0 1
device=RESISTOR
}
C 48400 43300 1 90 0 resistor-2.sym
{
T 48100 43500 5 10 1 1 90 0 1
refdes=16k
T 48050 43700 5 10 0 0 90 0 1
device=RESISTOR
}
C 48300 45500 1 90 0 switch-pushbutton-no-1.sym
{
T 48000 45900 5 10 1 1 90 0 1
refdes=change1
T 47700 45900 5 10 0 0 90 0 1
device=SWITCH_PUSHBUTTON_NO
}
N 48300 46500 48300 47100 4
N 48300 44200 48300 45500 4
N 45300 43300 45300 42600 4
N 45300 42600 48300 42600 4
N 48300 42600 48300 43300 4
N 47300 43300 47300 42600 4
N 46300 43300 46300 42600 4
N 48300 48000 48300 48700 4
N 48300 48700 45300 48700 4
N 45300 48700 45300 48000 4
N 46300 48000 46300 48700 4
N 47300 48000 47300 48700 4
C 49400 50400 1 180 0 terminal-1.sym
{
T 49090 49650 5 10 0 0 180 0 1
device=terminal
T 49090 49800 5 10 0 0 180 0 1
footprint=CONNECTOR 1 1
T 49150 50350 5 10 1 1 180 6 1
refdes=beta
}
N 48500 50300 47300 48700 4
C 47200 40500 1 0 0 common.sym
N 47300 40700 47300 42600 4
C 40800 40700 1 90 0 resistor-2.sym
{
T 40450 41100 5 10 0 0 90 0 1
device=RESISTOR
T 40500 40900 5 10 1 1 90 0 1
refdes=150k
}
C 40600 40500 1 0 0 common.sym
C 40500 44100 1 270 0 schottky-1.sym
{
T 41172 43778 5 10 0 0 270 0 1
device=DIODE
T 41000 43800 5 10 1 1 270 0 1
refdes=bat54
T 41332 43759 5 10 0 1 270 0 1
footprint=SOD80
}
C 41500 44100 1 270 0 schottky-1.sym
{
T 42172 43778 5 10 0 0 270 0 1
device=DIODE
T 42000 43800 5 10 1 1 270 0 1
refdes=bat54
T 42332 43759 5 10 0 1 270 0 1
footprint=SOD80
}
C 42500 44100 1 270 0 schottky-1.sym
{
T 43172 43778 5 10 0 0 270 0 1
device=DIODE
T 43000 43800 5 10 1 1 270 0 1
refdes=bat54
T 43332 43759 5 10 0 1 270 0 1
footprint=SOD80
}
C 43400 44100 1 270 0 schottky-1.sym
{
T 44072 43778 5 10 0 0 270 0 1
device=DIODE
T 43900 43800 5 10 1 1 270 0 1
refdes=bat54
T 44232 43759 5 10 0 1 270 0 1
footprint=SOD80
}
N 40700 41600 40700 43200 4
N 40700 42500 43600 42500 4
N 43600 42500 43600 43200 4
N 42700 43200 42700 42500 4
N 41700 43200 41700 42500 4
N 40700 45300 40700 44100 4
N 41700 45000 41700 44100 4
N 42700 44700 42700 44100 4
N 43600 44400 43600 44100 4
N 45300 45300 45300 45200 4
N 46300 45000 46300 44900 4
N 47300 44700 47300 44600 4
N 48300 44400 48300 44300 4
N 40700 42000 48700 42000 4
N 55800 42400 56000 42400 4
N 56000 42400 56000 41800 4
N 56000 41800 52100 41800 4
N 52100 41800 52100 45300 4
N 40700 45300 52100 45300 4
N 52600 43200 51900 43200 4
N 51900 43200 51900 45000 4
N 41700 45000 51900 45000 4
N 52600 42800 51800 42800 4
N 51800 42800 51800 44700 4
N 42700 44700 51800 44700 4
N 52600 42400 51700 42400 4
N 51700 42400 51700 44400 4
N 43600 44400 51700 44400 4
N 52600 44000 48700 44000 4
N 48700 42000 48700 44000 4