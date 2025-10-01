import nops from '../index'

async function testGetAllProcessNames() {
  const start = process.hrtime.bigint()
  const procs = await nops.GetAllProcessNames()
  const end = process.hrtime.bigint()
  const duration = Number(end - start) / 1000000
  console.log(`Test enumerating all processes: ${duration}ms`)
  console.log(`${procs.length} processes found, first: ${procs[0]}`)
  console.log('Test Done')
}

async function testGetAppWindowNames() {
  const start = process.hrtime.bigint()
  const windows = await nops.GetAppWindowNames()
  const end = process.hrtime.bigint()
  const duration = Number(end - start) / 1000000
  console.log(`Test enumerating all app windows: ${duration}ms`)
  console.log(`${windows.length} windows found, first executable: ${windows[0].executable}, title: ${windows[0].title}`)
  console.log('Test Done')
}

async function testGetAllWindowNames() {
  const start = process.hrtime.bigint()
  const windows = await nops.GetAllWindowNames()
  const end = process.hrtime.bigint()
  const duration = Number(end - start) / 1000000
  console.log(`Test enumerating all windows: ${duration}ms`)
  console.log(`${windows.length} windows found, first executable: ${windows[0].executable}, title: ${windows[0].title}`)
  console.log('Test Done')
}

;(async () => {
  console.log('Starting test...')
  nops.SetLogLevel(1)
  await testGetAllProcessNames()
  await testGetAppWindowNames()
  await testGetAllWindowNames()
  console.log('Test now running async')
})()
