const express = require('express');
const app = express();

const PORT = 3000;

app.use(express.static('public'));
app.use(express.json());

const addon = require('bindings')('addon.node');
const privileges = {
  0: 'Гость',
  1: 'Пользователь',
  2: 'Администратор',
};

app.post('/checkUser', (req, res) => {
  const { username } = req.body;
  const checkResult = addon.checkPrivileges(username);
  if (checkResult === -1) {
    return res.send(`Пользователя ${username} нет`);
  }
  res.send(
    `Пользователь ${username} имеет привилегию ${privileges[checkResult]}`
  );
});

app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
