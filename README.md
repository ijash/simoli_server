# Simoli Server


## Installation
This installation procedure is made for Ubuntu 18.04.
First, update the system.
```bash
sudo apt update -y && sudo apt full-upgrade -y && sudo apt autoremove -y && sudo apt clean -y && sudo apt autoclean -y
```
then install nodejs and all other dependencies.
```bash
curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash -
sudo apt-get install -y nodejs gcc g++ make build-essential
```
After Nodejs installed, then install nodered.
```bash
sudo npm install -g --unsafe-perm node-red
```
to make node-red available in startup, install pm2
```bash
sudo npm install -g pm2
```
And run these commands to generate startup script using systemd.
```bash
pm2 start /usr/bin/node-red -- -v
pm2 save
pm2 startup systemd
```
you will see something like this:
```bash
sudo env PATH=$PATH:/usr/bin /usr/lib/node_modules/pm2/bin/pm2 startup systemd -u vm --hp /home/vm
```
copy it and run as a shell command.


Now, we need to secure the node red with password. To do that, first we will need to install node red admin for generating password hash.
```bash
sudo npm install -g node-red-admin --unsafe-perm
```
To remotely administer a Node-RED instance, the tool must first be pointed at the Node-RED instance you want it to access. By default, it assumes ```http://localhost:1880```. To change that, use the ```target``` command:
```bash
node-red-admin target http://node-red.example.com/admin
```
More info about node red admin are on [this guide](https://nodered.org/docs/user-guide/node-red-admin).

Then, to generate password hash, use:
```bash
node-red-admin hash-pw
```
and enter desired password.
then copy the output.

Go to ```settings.js``` file on ```~/.node-red``` and open it with text editor.

Then, go tho the line where the ```//Securing Node-RED``` is. Below it, Uncomment or add some lines to match something like this:
```js
adminAuth: {
    type: "credentials",
    users: [
        {
            username: "admin",
            password: "$2a$08$zZWtXTja0fB1pzD4sHCMyOCMYz2Z6dNbM6tl8sJogENOMcxWV9DN.",
            permissions: "*"
        },
        {
            username: "george",
            password: "$2b$08$wuAqPiKJlVN27eF5qJp.RuQYuy6ZYONW7a/UWYxDTtwKFCdB8F19y",
            permissions: "read"
        }
    ]
}

```
configure the password using the previously copied hashed password and save. More on on this [security page](https://nodered.org/docs/user-guide/runtime/securing-node-red).

reboot for now using 
```bash
sudo reboot now
```
node-red should now be using password.

## Nginx for reverse proxy

Install nginx
```bash
sudo apt install curl gnupg2 ca-certificates lsb-release -y
echo "deb http://nginx.org/packages/ubuntu `lsb_release -cs` nginx"     | sudo tee /etc/apt/sources.list.d/nginx.list
curl -fsSL https://nginx.org/keys/nginx_signing.key | sudo apt-key add -
sudo apt-key fingerprint ABF5BD827BD9BF62
sudo apt update
sudo apt install nginx
```
to check nginx status:
```bash
sudo systemctl status nginx
```
now open nginx config using with administrative privileges.
```bash
sudo nano /etc/nginx/conf.d/default.conf
```
add these under inside `location /` block.
```nginx
location /nodered/ {
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection "upgrade";
    proxy_set_header Host $http_host;
    proxy_http_version 1.1;
    proxy_pass http://localhost:1880;
}
```
these are optional, configure `/your_location/` to fit your needs.
```nginx
location /your_location/ {
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection "upgrade";
    proxy_set_header Host $http_host;
    proxy_http_version 1.1;
    proxy_pass http://localhost:1880/your_location;
}
```
to test the nginx configuration file, run:
```bash
sudo nginx -t
```
if successful, do restart nginx using the following command:
```bash
sudo systemctl restart nginx
```
