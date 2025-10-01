const nops = require('../index.js')

async function TestGetAllProcessNames() {
  console.time('Test enumerating all processes')
  const procs = nops.GetAllProcessNames()
  console.timeEnd('Test enumerating all processes')
  console.log(`${procs.length} processes found, first: ${procs[0]}`)
  console.log('Test Done')
}

async function TestGetAppWindowNames() {
  console.time('Test enumerating all app windows')
  const windows = nops.GetAppWindowNames()
  console.timeEnd('Test enumerating all app windows')
  console.log(`${windows.length} windows found, first: ${windows[0]}`)
  console.log('Test Done')
}

async function TestGetAllWindowNames() {
  console.time('Test enumerating all windows')
  const windows = nops.GetAllWindowNames()
  console.timeEnd('Test enumerating all windows')
  console.log(`${windows.length} windows found, first: ${windows[0]}`)
  console.log('Test Done')
}

console.log('Starting test...')
nops.SetLogLevel(400)
TestGetAllProcessNames()
TestGetAppWindowNames()
TestGetAllWindowNames()
console.log('Test now running async')
