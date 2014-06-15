scp dis pi@192.168.1.7:/home/pi/homeis

ssh pi@192.168.1.7 <<'ENDSSH'
sudo gdbserver :2345 homeis/dis
ENDSSH

