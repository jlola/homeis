scp dis linaro@192.168.1.5:/home/linaro/homeis

ssh -T linaro@192.168.1.5 <<'ENDSSH'
sudo gdbserver :2345 homeis/dis
ENDSSH

