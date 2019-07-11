#!/bin/bash

if [ "$EUID" -eq 0 ]
then echo "Please do not run as root..."
    exit
fi

function enterpass() {
    unset passchars
    prompt=""
    while IFS= read -p "$prompt" -r -s -n 1 char
    do
        if [[ $char == $'\0' ]]
        then
            break
        fi
        prompt='*'
        passchars+="$char"
    done
    returnpass=$passchars
}

function createpass() {
    local exit_loop=0
    local returnpass
    local pass1="undefined1"
    local pass2="undefined2"
    while [ $exit_loop != 1 ]
    do
        echo  "enter a password for $1.."
        enterpass $1
        pass1=$returnpass
        echo  "confirm the $1 password.."
        enterpass $1
        pass2=$returnpass
        if [ $pass1 == $pass2 ]
        then
            let "exit_loop = 1"
            func_result=$pass1
        else
            echo "password inconsistent.."
            echo "try again.."
        fi
    done
}

echo "Enter desired Node-Red ID:"
read id_nodered
createpass "Node-RED"
pass_nodered=$func_result

# echo "Enter desired MySQL ID:"
# read id_mysql
# createpass "MySQL"
# pass_mysql=$func_result

# echo "Enter desired phpMyAdmin ID:"
# read id_phpmyadmin
# createpass "phpMyAdmin"
# pass_phpmyadmin=$func_result

# echo "Enter desired xxxxxxxxxxx ID:"
# read id_xxxxxxxxxxx
# createpass "xxxxxxxxxxx"
# pass_xxxxxxxxxxx=$func_result


curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash -
sudo apt update -y && sudo apt full-upgrade -y && sudo apt autoremove -y && sudo apt clean -y && sudo apt autoclean -y
sudo apt install -y nodejs gcc g++ make build-essential nginx mysql-server

sudo npm install -g --unsafe-perm node-red node-red-admin
sudo npm install -g pm2





pm2 start /usr/bin/node-red -- -v
pm2 save
systemd_command=$(pm2 startup systemd | sed '3!d')
eval $systemd_command




sudo chown -R $USER:$GROUP ~/.npm
sudo chown -R $USER:$GROUP ~/.config


node-red-admin target http://localhost:1880


hasil=$(echo "$pass_nodered" | node-red-admin hash-pw)
hashed_pass_nodered=`echo $hasil | grep -oP '(?<=\[22m\s).*'`

# echo '
# adminAuth: {
#     type: "credentials",
#     users: [
#         {
#             username: "'$id_nodered'",
#             password: "'$hashed_pass_nodered'",
#             permissions: "*"
#         }
#     ]
# },
# httpStatic: "/home/'$USER'/node-red-static/",
# '
# >> $HOME/.node-red/settings.js

#####HARUSNYA SEBELUM EXPORT, PAKE sed


sudo systemctl enable nginx
sudo systemctl start nginx
sudo chown www-data:www-data /usr/share/nginx/html -R







# mysql --user="$user" --password="$password" --database="$database" --execute="DROP DATABASE $user; CREATE DATABASE $database;"









































# echo "----------------------------"
# echo "Please take note on this:"
# echo "[ID | PASS]"
# echo "Node-RED"
# echo "[$id_nodered | $pass_nodered]"
# echo "MySQL"
# echo "[$pass_mysql | $id_mysql]"
# echo "phpMyAdmin"
# echo "[$id_phpmyadmin | $pass_phpmyadmin]"
# echo "xxxxxxxxxxx"
# echo "[$id_xxxxxxxxxxx | $pass_xxxxxxxxxxx]"


















