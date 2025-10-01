import nops from '../index'

async function TestGetAllProcessNames() {
  const start = process.hrtime.bigint()
  const procs = nops.GetAllProcessNames()
  const end = process.hrtime.bigint()
  const duration = Number(end - start) / 1000000
  console.log(`Test enumerating all processes: ${duration}ms`)
  console.log(`${procs.length} processes found, first: ${procs[0]}`)
  console.log('Test Done')
}

function testGetAppWindowNames() {
  const start = process.hrtime.bigint()
  const windows = nops.GetAppWindowNames()
  const end = process.hrtime.bigint()
  const duration = Number(end - start) / 1000000
  console.log(`Test enumerating all app windows: ${duration}ms`)
  console.log(`${windows.length} windows found, first executable: ${windows[0].executable}, title: ${windows[0].title}`)
  console.log('Test Done')
}

function testGetAllWindowNames() {
  const start = process.hrtime.bigint()
  const windows = nops.GetAllWindowNames()
  const end = process.hrtime.bigint()
  const duration = Number(end - start) / 1000000
  console.log(`Test enumerating all windows: ${duration}ms`)
  console.log(`${windows.length} windows found, first executable: ${windows[0].executable}, title: ${windows[0].title}`)
  console.log('Test Done')
}

console.log('Starting test...')
nops.SetLogLevel(1)
TestGetAllProcessNames()
testGetAppWindowNames()
testGetAllWindowNames()
console.log('Test now running async')
