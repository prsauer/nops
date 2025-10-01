const fs = require('fs')
const path = require('path')

const packageName = 'nops.node'
const distRoot = path.resolve(__dirname, 'dist')

// Clean the dist directory if it exists.
if (fs.existsSync(distRoot)) {
  fs.rmSync(distRoot, { recursive: true, force: true })
}

// Remake the dist directory structure.
fs.mkdirSync(distRoot)

// Copy the compiled .node file.
const addonSrc = path.resolve(__dirname, 'build', 'Release', packageName)
const addonDest = path.join(distRoot, packageName)
fs.copyFileSync(addonSrc, addonDest)
