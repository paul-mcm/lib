#!/bin/ksh

FPATH='./'
ip='192.12.0.33/23'

echo "IP: $ip\n"

echo "Test parse_octets function" 
set -A octs $(parse_octets $ip)
for i in ${octs[@]}
do
    echo $i
done

echo "=========================="
echo "Test find_octet function"
echo $(find_octet $ip 2)

echo "=========================="
echo "Test ipvton function"
echo "$(ipv4ton $ip)"

echo "=========================="
echo "Test validate_ipv4_cidr function"

validate_ipv4_cidr $ip
if [[ $? -ne 0 ]] 
then
    echo "Bad addr"
    return 1
fi



