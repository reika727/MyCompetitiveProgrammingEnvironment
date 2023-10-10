import tough from 'tough-cookie'

async function getLoginCookieJar(userName: string, password: string) {
  const url = 'https://atcoder.jp/login'

  const temporaryCookies = await fetch(url)
    .then((response) => response.headers.getSetCookie())
  const csrfToken = /csrf_token:(.+?=)/.exec(decodeURIComponent(temporaryCookies[1]))?.[1]!

  const headers = new Headers
  for (const temporaryCookie of temporaryCookies) {
    headers.append('Cookie', temporaryCookie)
  }

  const body = new FormData
  body.append('username', userName)
  body.append('password', password)
  body.append('csrf_token', csrfToken)

  const setCookie = fetch(url, {
    method: 'POST',
    redirect: 'manual',
    headers,
    body
  }).then((response) => response.headers.getSetCookie())

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
