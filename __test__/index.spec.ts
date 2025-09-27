import test from 'ava'

import { plus100, plus200, getSystemInfo } from '../index'

test('sync function from native code', (t) => {
  const fixture = 42
  t.is(plus100(fixture), fixture + 100)
})

test('x function from native code', (t) => {
  const fixture = 42
  t.is(plus200(fixture), fixture + 200)
  console.log(getSystemInfo().length)
})
