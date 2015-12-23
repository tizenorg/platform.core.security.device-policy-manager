# PAM unshare
This is [PAM(Pluggable Authentication Modules)](http://www.linux-pam.org/).
This can be used for para-virtualization instead of LXC.
This can be useful if you want to make small environment just using multi-user.
For example, if you put this module in "systemd-user" PAM service, all processes in systemd user session will be isolated from another session.
And if you logins as the same user by using this module, you can use the same namespace.

## Code formatting
We use [astyle](http://astyle.sourceforge.net/) for code formatting (Use the latest version)
You can find the options file in the root of the project.

For example to format all .cpp and .hpp files run in the project directory:

    astyle --options=./astylerc --recursive ./*.c ./*.h
