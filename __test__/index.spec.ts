import test from 'ava'

import { getSystemInfo } from '../index'

test('x function from native code', (t) => {
  console.time('getSystemInfo')
  const sysProcs = getSystemInfo()

  console.timeEnd('getSystemInfo')
  console.log(sysProcs.slice(0, 10))
  t.true(sysProcs.length > 5)
})
