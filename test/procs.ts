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
  console.log('Testing PioneerGame (should be running with some UDP ports)...')
  const ports = await nops.GetProcessUdpPorts('PioneerGame.exe')
  const end = process.hrtime.bigint()
  const duration = Number(end - start) / 1000000
  console.log(`Test UDP port count: ${duration}ms`)
  if (ports === null) {
    console.error('Error: Process not found (PioneerGame.exe should be running)')
  } else {
    console.log(`PioneerGame.exe UDP ports: ${ports.length}`)
    if (ports.length > 0) {
      console.log(`Ports: ${ports.join(', ')}`)
      console.log('Successfully found PioneerGame with UDP ports!')
    } else {
      console.error(`Error: Expected non-zero ports for PioneerGame, but got ${ports.length}`)
    }
  }
  console.log('Test Done')
}

async function testNonExistentProcess() {
  console.log('Testing non-existent process...')
  const ports = await nops.GetProcessUdpPorts('nonexistentprocess.exe')
  if (ports === null) {
    console.log('Successfully returned null for non-existent process')
  } else {
    console.error(`Error: Expected null, but got ${ports}`)
  }
  console.log('Test Done')
}

async function testProcessWithNoPorts() {
  console.log('Testing process that exists but may have no UDP ports...')
  const ports = await nops.GetProcessUdpPorts('explorer.exe')
  if (ports === null) {
    console.error('Error: Process not found (explorer.exe should be running)')
  } else {
    console.log(`explorer.exe UDP ports: ${ports.length}`)
    if (ports.length === 0) {
      console.log('Successfully returned empty array for process with no UDP ports')
    } else {
      console.log(`Ports: ${ports.join(', ')}`)
    }
  }
  console.log('Test Done')
}

async function testDiscord() {
  console.log('Testing Discord (should be running but have no UDP ports)...')
  const ports = await nops.GetProcessUdpPorts('Discord.exe')
  if (ports === null) {
    console.error('Error: Process not found (Discord.exe should be running)')
  } else {
    console.log(`Discord.exe UDP ports: ${ports.length}`)
    if (ports.length === 0) {
      console.log('Successfully returned empty array for Discord with no UDP ports')
    } else {
      console.log(`Ports: ${ports.join(', ')}`)
    }
  }
  console.log('Test Done')
}

;(async () => {
  console.log('Starting test...')
  nops.SetLogLevel(400)
  // await testGetAllProcessNames()
  // await testGetAppWindowNames()
  // await testGetAllWindowNames()
  await testUdpPorts()
  await testNonExistentProcess()
  await testProcessWithNoPorts()
  await testDiscord()
  console.log('Test now running async')
})()
