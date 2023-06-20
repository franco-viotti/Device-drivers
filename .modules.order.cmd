cmd_/home/pi/Device-drivers/modules.order := {   echo /home/pi/Device-drivers/perp_cdd.ko; :; } | awk '!x[$$0]++' - > /home/pi/Device-drivers/modules.order
