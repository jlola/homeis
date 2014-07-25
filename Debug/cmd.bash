scp dis linaro@192.168.1.9:/home/linaro/homeis

ssh linaro@192.168.1.9 <<'ENDSSH'
sudo gdbserver :2345 homeis/dis
ENDSSH

