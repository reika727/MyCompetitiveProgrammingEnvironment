import { CookieJar } from 'jsdom'

const [ userName, password ] = process.argv.slice(2)

const url = new URL('https://atcoder.jp/login')

const temporaryCookies =
  await fetch(url)
  .then(response => response.headers.getSetCookie())

const csrfToken =
  temporaryCookies
  .values()
  .map(temporaryCookie => /csrf_token:(.+?=)/.exec(decodeURIComponent(temporaryCookie))?.[1])
  .find(value => value !== undefined)

if (csrfToken === undefined) {
  throw new Error("csrf_token was not found.")
}

const body = new FormData
body.set('username', userName)
body.set('password', password)
body.set('csrf_token', csrfToken)

const loginResponse = await fetch(url, {
  method: 'POST',
  redirect: 'manual',
  headers: new Headers(
    temporaryCookies.map(
      temporaryCookie => ['Cookie', temporaryCookie]
    )
  ),
  body
})

const cookieJar = new CookieJar()
loginResponse.headers.getSetCookie().forEach(cookie => cookieJar.setCookie(cookie, url))

const revelFlash = await cookieJar.store.findCookie(url.hostname, '/', 'REVEL_FLASH')
if (decodeURIComponent(revelFlash.value) !== `\0success:Welcome,+${userName}.\0`) {
  throw new Error('login failed.')
}

console.log(JSON.stringify(cookieJar, null, 2))
