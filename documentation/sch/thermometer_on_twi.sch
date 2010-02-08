v 20081231 1
C 40000 40000 0 0 0 title-B.sym
T 49500 40900 9 10 1 0 0 0 2
Thermometer on TWI

C 45100 41600 1 0 0 ATmega16L-1.sym
{
T 48100 50200 5 10 1 1 0 6 1
refdes=PDIP40
T 45500 50350 5 10 0 0 0 0 1
device=ATmega16L_DIP
T 45500 50550 5 10 0 0 0 0 1
footprint=DIP40
T 45500 51750 5 10 0 0 0 0 1
symversion=2.0
}
C 55000 43200 1 0 0 connector4-2.sym
{
T 55700 45300 5 10 1 1 0 6 1
refdes=Thermometer socket
T 55300 45250 5 10 0 0 0 0 1
device=CONNECTOR_4
T 55300 45450 5 10 0 0 0 0 1
footprint=SIP4N
}
C 52000 50400 1 270 0 terminal-1.sym
{
T 52750 50090 5 10 0 0 270 0 1
device=terminal
T 52600 50090 5 10 0 0 270 0 1
footprint=CONNECTOR 1 1
T 52050 50150 5 10 1 1 270 6 1
refdes=gamma
}
C 52200 48600 1 90 0 resistor-2.sym
{
T 51850 49000 5 10 0 0 90 0 1
device=RESISTOR
T 51900 48800 5 10 1 1 90 0 1
refdes=510
}
C 53100 47900 1 0 0 capacitor-4.sym
{
T 53300 49000 5 10 0 0 0 0 1
device=POLARIZED_CAPACITOR
T 53300 48400 5 10 1 1 0 0 1
refdes=200u
T 53300 48600 5 10 0 0 0 0 1
symversion=0.1
}
C 52200 45800 1 90 0 resistor-2.sym
{
T 51850 46200 5 10 0 0 90 0 1
device=RESISTOR
T 51900 46000 5 10 1 1 90 0 1
refdes=10k
}
C 51100 45800 1 90 0 resistor-2.sym
{
T 50750 46200 5 10 0 0 90 0 1
device=RESISTOR
T 50800 46000 5 10 1 1 90 0 1
refdes=10k
}
C 55600 48000 1 90 0 common.sym
N 55400 48100 54000 48100 4
N 48400 42900 48900 42900 4
N 48900 44800 55000 44800 4
N 48900 42900 48900 44800 4
N 49500 44000 55000 44000 4
N 49500 44000 49500 42500 4
N 49500 42500 48400 42500 4
T 55900 44000 9 10 1 0 0 0 4
Data


Clock
T 56000 43600 9 10 1 0 0 0 4
+


-
N 51000 46700 51000 47000 4
N 51000 47000 52100 47000 4
N 52100 46700 52100 48600 4
N 51000 45800 51000 44800 4
N 52100 45800 52100 44000 4
N 55000 44400 53600 44400 4
N 53600 44400 53600 47000 4
N 53600 47000 52100 47000 4
C 54900 42900 1 0 0 common.sym
N 55000 43100 55000 43600 4
N 53100 48100 52100 48100 4
T 53000 49000 9 10 1 0 0 0 1
Noise filter