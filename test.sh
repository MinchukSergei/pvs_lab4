sudo rm -f /dev/calc*
sudo rmmod calc
make
sudo insmod ./calc.ko

sudo mknod /dev/calc0 c 300 0
sudo mknod /dev/calc1 c 300 1
sudo mknod /dev/calc2 c 300 2
sudo mknod /dev/calc3 c 300 3

sudo chmod a+r+w /dev/calc*

sudo echo "5 + 8 ="
sudo echo "5" > /dev/calc0
sudo echo "8" > /dev/calc1
sudo echo "+" > /dev/calc2
cat /dev/calc3

sudo echo "3 * 5 ="
sudo echo "3" > /dev/calc0
sudo echo "5" > /dev/calc1
sudo echo "*" > /dev/calc2
cat /dev/calc3

sudo echo "20 - 35 ="
sudo echo "20" > /dev/calc0
sudo echo "35" > /dev/calc1
sudo echo "-" > /dev/calc2
cat /dev/calc3

sudo echo "10 / 2 ="
sudo echo "10" > /dev/calc0
sudo echo "2" > /dev/calc1
sudo echo "/" > /dev/calc2
cat /dev/calc3

sudo echo "10 / 0 ="
sudo echo "10" > /dev/calc0
sudo echo "0" > /dev/calc1
sudo echo "/" > /dev/calc2
cat /dev/calc3
