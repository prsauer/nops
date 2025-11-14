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
  const maybeWoWExe = windows.find((window) => window.executable.toLowerCase().includes('fcadefbneo.exe'))
  console.log(`Test enumerating APP windows: ${duration}ms`)
  console.log(`${windows.length} windows found, first executable: ${windows[0].executable}, title: ${windows[0].title}`)
  console.log(`Maybe Target executable: ${maybeWoWExe?.executable}, title: ${maybeWoWExe?.title}`)
  console.log('Test Done')
}

async function testGetAllWindowNames() {
  const start = process.hrtime.bigint()
  const windows = await nops.GetAllWindowNames()
  const end = process.hrtime.bigint()
  const duration = Number(end - start) / 1000000
  const maybeWoWExe = windows.find((window) => window.executable.toLowerCase().includes('fcadefbneo.exe'))
  console.log(`Test enumerating ALL windows: ${duration}ms`)
  console.log(`${windows.length} windows found, first executable: ${windows[0].executable}, title: ${windows[0].title}`)
  console.log(`Maybe Target executable: ${maybeWoWExe?.executable}, title: ${maybeWoWExe?.title}`)
  console.log('Test Done')
}

async function testUdpPorts() {
  const start = process.hrtime.bigint()
  const ports = await nops.GetProcessUdpPorts('PioneerGame.exe')
  const end = process.hrtime.bigint()
  const duration = Number(end - start) / 1000000
  console.log(`Test UDP port count: ${duration}ms`)
  console.log(`PioneerGame.exe UDP ports: ${ports.length}`)
  console.log(`Ports: ${ports.join(', ')}`)
  if (ports.length !== 47) {
    console.error(`Expected 47 ports, but got ${ports.length}`)
  } else {
    console.log('UDP port count test passed!')
  }
  console.log('Test Done')
}

;(async () => {
  console.log('Starting test...')
  nops.SetLogLevel(400)
  await testGetAllProcessNames()
  await testGetAppWindowNames()
  await testGetAllWindowNames()
  await testUdpPorts()
  console.log('Test now running async')
})()
