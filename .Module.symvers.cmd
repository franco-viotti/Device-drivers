cmd_/home/pi/Device-drivers/Module.symvers := sed 's/\.ko$$/\.o/' /home/pi/Device-drivers/modules.order | scripts/mod/modpost -m -a  -o /home/pi/Device-drivers/Module.symvers -e -i Module.symvers   -T -
