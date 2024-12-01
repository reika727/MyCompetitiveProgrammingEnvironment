import tough from 'tough-cookie'

async function getLoginCookieJar(userName: string, password: string) {
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

  const cookieJar = new tough.CookieJar()
  await fetch(url, {
    method: 'POST',
    redirect: 'manual',
    headers: new Headers(
      temporaryCookies.map(
        temporaryCookie => ['Cookie', temporaryCookie]
      )
    ),
    body
  })
  .then(response => response.headers.getSetCookie())
  .then(setCookie => setCookie.forEach(
      cookie => cookieJar.setCookie(cookie, url)
    )
  )

  const revelFlash =
    await cookieJar.store
    .findCookie(url.hostname, '/', 'REVEL_FLASH')
    .then((cookie: Cookie) => decodeURIComponent(cookie.value))

  if (revelFlash === `\0success:Welcome,+${userName}.\0`) {
    return cookieJar
  } else {
    throw new Error('login failed.')
  }
}

const [ userName, password ] = process.argv.slice(2)
console.log(JSON.stringify(await getLoginCookieJar(userName, password), null, 2))
