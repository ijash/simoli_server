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
sudo apt install -y nodejs gcc g++ make build-essential
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

### Change NPM Ownership

Some Node-RED plugins require npm to install, it'll give error if npm is not owned by the user. Therefore, restore ownership of the user's npm related folders, to the current user, like this:

```bash
sudo chown -R $USER:$GROUP ~/.npm
sudo chown -R $USER:$GROUP ~/.config
```

### Securing Node-RED Page

Now, we need to secure the Node-RED with password. To do that, first we will need to install Node-RED admin for generating password hash.

```bash
sudo npm install -g node-red-admin --unsafe-perm
```

To remotely administer a Node-RED instance, the tool must first be pointed at the Node-RED instance you want it to access. By default, it assumes `http://localhost:1880`. To change that, use the `target` command:

```bash
node-red-admin target http://localhost:1880
```

More info about Node-RED admin are on [this guide](https://nodered.org/docs/user-guide/node-red-admin).

Then, to generate password hash, use:

```bash
node-red-admin hash-pw
```

and enter desired password.
then copy the output.

Go to `settings.js` file on `~/.node-red` and open it with text editor.

```bash
nano ~/.node-red/settings.js
```

Then, go tho the line where the `//Securing Node-RED` is. Below it, Uncomment or add some lines to match something like this:

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

configure the password using the previously copied hashed password and save. More on [this security page](https://nodered.org/docs/user-guide/runtime/securing-node-red).

reboot for now using

```bash
sudo reboot now
```

node-red should now be using password.

### Setting up static content for Node-RED

Back to `settings.js` file on `~/.node-red` and open it with text editor.

```bash
nano ~/.node-red/settings.js
```

Uncomment `httpStatic:` line or add following line:

```js
 httpStatic: '/home/username/node-red-static/',
```

Value of `'/home/username/node-red-static/'` can be changed according to your needs, but make sure it's not on the system folder.

Now the static files can be accessed from the `root` path of the url in the web browser. example:

```url
http://localhost:1880/directory/yourstaticfile.jpg
```

## Nginx Installation

Install nginx

```bash
sudo apt install nginx -y
```

and make sure nginx is available on boot:

```bash
sudo systemctl enable nginx
sudo systemctl start nginx
```

to check nginx status:

```bash
sudo systemctl status nginx
```

Make `www-data` (Nginx user) as the owner of web directory. By default it’s owned by the root user.

```bash
sudo chown www-data:www-data /usr/share/nginx/html -R
```

## MySQL installation

using guide from [here](https://www.digitalocean.com/community/tutorials/how-to-install-mysql-on-ubuntu-18-04),
Install MySQL with these commands.

```bash
sudo apt update
sudo apt install mysql-server -y
sudo mysql_secure_installation
```

then, enter mysql with command:

```bash
sudo mysql
```

and enter:

```sql
ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'password';
```

please note that `'password'` is your own preferences.

Then, run `FLUSH PRIVILEGES` which tells the server to reload the grant tables and put your new changes into effect:

```sql
FLUSH PRIVILEGES;
```

then, exit by typing command:

```sql
exit
```

## Installing PHP & phpMyAdmin

Configuring Nginx
according to this [guide](https://linuxize.com/post/how-to-install-phpmyadmin-with-nginx-on-ubuntu-18-04/), install all the php stuff to install the phpmyadmin client.

```bash
sudo apt install php-fpm php-mysql phpmyadmin -y
```

The installer will ask you choose the web server, skip it by pressing`tab` and `Enter`.
Next, about the _dbconfig-common_ tool. Select `Yes` and hit `Enter`. Then, enter a password of your choice.

## Nginx setting

### Server Configuration

Configure the server setting:

```bash
sudo nano /etc/nginx/sites-available/default
```

Paste the following content:

```nginx
server {
        listen 80;
        root /var/www/html;
        index index.php index.html index.htm index.nginx-debian.html;
        server_name ;
        include snippets/phpmyadmin.conf;


        location / {
                try_files $uri $uri/ =404;
                include snippets/nodered.conf;
        }

        location ~ \.php$ {
                include snippets/fastcgi-php.conf;
                fastcgi_pass unix:/var/run/php/php7.2-fpm.sock;
        }

        location ~ /\.ht {
                deny all;
        }
}
```

### phpMyAdmin Nginx Configuration

Create phpmyadmin settings for nginx

```bash
sudo nano /etc/nginx/snippets/phpmyadmin.conf
```

Copy and paste this:

```nginx
location /phpmyadmin {
    root /usr/share/;
    index index.php index.html index.htm;
    location ~ ^/phpmyadmin/(.+\.php)$ {
        try_files $uri =404;
        root /usr/share/;
        fastcgi_pass unix:/run/php/php7.2-fpm.sock;
        fastcgi_index index.php;
        fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
        include /etc/nginx/fastcgi_params;
    }

    location ~* ^/phpmyadmin/(.+\.(jpg|jpeg|gif|css|png|js|ico|html|xml|txt))$ {
        root /usr/share/;
    }
}
```

### Node-RED Nginx Configuration

Create Node-RED settings for nginx

```bash
sudo nano /etc/nginx/snippets/nodered.conf
```

Paste the following content:

```nginx
location /nodered/ {
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection "upgrade";
    proxy_set_header Host $http_host;
    proxy_http_version 1.1;
    proxy_pass http://localhost:1880/;
}
```

these are **optional**, configure `/your_location/` to fit your needs.

```nginx
location /your_location/ {
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection "upgrade";
    proxy_set_header Host $http_host;
    proxy_http_version 1.1;
    proxy_pass http://localhost:1880/your_location/;
}
```

these too are **ptional** to make nginx read the `sites-available` by linking the directory to `sites-enabled`.

```bash
sudo ln -s /etc/nginx/sites-available/default /etc/nginx/sites-enabled/
```

### finishing it up

To test the nginx configuration file, run:

```bash
sudo nginx -t
```

if successful, do restart nginx using the following command:

```bash
sudo systemctl restart nginx
```

now, access the host IP with `/nodered` path from the web browser without port number.
_Note: Now the static file will be relative to `/nodered/` path_
