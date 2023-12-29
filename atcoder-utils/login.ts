import tough from 'tough-cookie'

async function getLoginCookieJar(userName: string, password: string) {
  const url = 'https://atcoder.jp/login'

  const temporaryCookies = await fetch(url)
    .then(response => response.headers.getSetCookie())
  const csrfToken = /csrf_token:(.+?=)/.exec(decodeURIComponent(temporaryCookies[1]))?.[1]!

  const body = new FormData
  body.set('username', userName)
  body.set('password', password)
  body.set('csrf_token', csrfToken)

  const setCookie = fetch(url, {
    method: 'POST',
    redirect: 'manual',
    headers: new Headers(
      temporaryCookies.map(
        temporaryCookie => ['Cookie', temporaryCookie]
      )
    ),
    body
  }).then(response => response.headers.getSetCookie())

  const cookieJar = new tough.CookieJar()
  for (const cookie of await setCookie) {
    cookieJar.setCookie(cookie, url)
  }

  const revelFlash = decodeURIComponent(cookieJar.getCookiesSync(url)[0].value)

  if (revelFlash == `\0success:Welcome,+${userName}.\0`) {
    return cookieJar
  } else {
    throw new Error('login failed.')
  }
}

const [ userName, password ] = process.argv.slice(2)
console.log(JSON.stringify(await getLoginCookieJar(userName, password), null, 2))
