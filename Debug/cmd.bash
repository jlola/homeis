scp dis pi@192.168.1.10:/home/pi/homeis

ssh pi@192.168.1.10 <<'ENDSSH'
sudo gdbserver :2345 homeis/dis
ENDSSH

