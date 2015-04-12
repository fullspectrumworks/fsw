README for simple vagrant ansible 3 node sandbox
================================================

## Intent

This sandbox is designed to build and configure 3
VirtualBox instances or nodes and have them NAT'ed
through the host and networked together (10.0.30.x).

In the final running state this will be the result:

- Host server with an outside connection (wifi or eth).
  - node1 - virtually networked as 10.0.30.21
  - node2 - virtually networked as 10.0.30.22
  - node3 - virtually networked as 10.0.30.23


This project is limited and currently (2015-04) only
developed for a Debian family host server. Additionally
each node will be a Debian/Ubuntu instance.

The host server must be capable with hardware virtualization.
This can be determined with this command

```bash
egrep --color "vmx|svm" /proc/cpuinfo
```

If present one of the flags above will be highlighted for each CPU.

You should have adequate memory and disk space to run 3 VirtualBox
Servers.

Required installed packages:

- VirtualBox
- vagrant

Optional installed packages:
- ansible (for CMS experimentation if desired)

## Vagrant

Vagrant must be installed and it depends on VirtualBox being installed.
Vagrant is software to ease building out and configuring virtual 
development environments. For this simple case we are using
VirtualBOx but vagrant can be used with other virtualization software.


## Ansible

An ansible playfile.yml has been provided in this package to experiment with
if you are brave. It is optional but does demonstrate some of the power in
the ansible CMS (Configuration Management Software) suite. The current development
path has gone past agile and now moved towards a DevOps strategy for short cycle
frequent deployment development cycles. Ansible is rapidly becoming a major
player in this strategy as it is realtively simple, easily made repeatable,
can be run on any server, and uses the universally common SSH protocol.

## How to use this **vagrant** in this sandbox

Untar the package whereever you desire - keep in mind that adequate disk space
will be needed for building out the 3 virtual guest nodes.

This will create this structure:

````
+-----sandbox 
|     +-----hosts
|     +-----playbook.yml
|     +-----README.md
|     +-----Vagrantfile

```

The `hosts` file is strictly for ansible if you decide to try it. It denotes
the IP addresses of the 3 virtual nodes for provisioning.

The `playbook.yml` file is also strictly for ansible experimentation. You can
edit it as desired keeping in mind that it a yml file (indicate by the extension
name and the required beginning three dashes at the top of the file). The yml
syntax is very sensitive to indentation and format so don't get frustrated with
its finicky demands.

The only needed file here is the Vagrantfile. The filename must stay as it is named.
The vagrant software looks for that name. The Vagrantfile is a configuration file
that stipulates the virtual node details desired. The one provided here is a slightly
more complicated than a base build so that one can appreciate some of the power
vagrant provides.

Here is the command to run it. Make sure your working directory is where the
Vagrantfile is (ie sandbox/). You do not have to use sudo or be root.

```
vagrant up
```

This will download the ubuntu image and bring up 3 nodes. To log into these node
you can use vagrant or use SSH directly. The default login user is "vagrant" and the
default password is also "vagrant".

Here are 2 possible ways to reach node1

```
vagrant ssh node1
# or
ssh vagrant@10.0.30.21
```

To shut down all these instances/nodes again you can use vagrant.

```
vagrant halt
# or to shut down just node1
vagrant halt node1
```

## Optional: How to use **ansible** in this sandbox

The ansible playbook.yml file provided is designed to install VirtualBox,
vagrant, and then bring up the 3 nodes with vagrant, and then provision/configure
the nodes for you. There are sections commented out that will actually build out
LAMP servers for you if you want.
__*WARNING*__: this ansible playbook.yml has not been fully tested! There will be 
things that could fail.

But, having read the above warning, and remaining couragous in the face of failure you
can choose the risk of running following commands.

```
sudo apt-get install ansible # only needs to be run once
cd .../sandbox/
ansible playbook.yml -i hosts
```

## Resources 

- [html://www.vagrantup.com]
- [html://docs.ansible.com]

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Enjoy!
-geoff-


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Note: This file is in markdown format.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
