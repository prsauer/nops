const path = require('path');

process.env.Path += ';';
process.env.Path += path.resolve(__dirname, 'dist', 'bin').replace('app.asar', 'app.asar.unpacked');

const packageName = 'nops.node';
const nops = require(`./dist/${packageName}`);
module.exports = nops;
