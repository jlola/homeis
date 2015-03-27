scp dis cubie@teplomer.juricekpavel.net:/home/cubie/dis
ssh -T cubie@teplomer.juricekpavel.net <<'ENDSSH'
gdbserver :2345 dis/dis
ENDSSH

