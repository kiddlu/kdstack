#!/bin/bash 

#set -x
function get_pc_config() {
    echo "PCI Devices,"
    lspci | sed s/" "/','/1
    echo "CPU Spec,"
    lscpu | sed s/":"/','/1
}

function ps_exist() {
    local ps_name=$1
    pidof "$ps_name" && echo "These $ps_name were found." || echo "No $ps_name found"
}

echo -en "Getting system details..."
get_pc_config > pci.cvs
echo "Done."
ls -l pci.cvs

ps_exist sshd | tee sshd.log
ps_exist smbd | tee smbd.log
ps_exist zsh  | tee zsh.log 
trap "rm -f sshd.log smbd.log zsh.log pci.cvs" EXIT
