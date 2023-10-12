import fs from 'fs'
import jsdom from 'jsdom'

const fetch_retry_after = async (ms: number, url: string, options = {}) => {
  while (true) {
    const response = await fetch(url, options)
    if (response.status == 429) {
      await new Promise(_ => setTimeout(_, ms))
    } else {
      return response
    }
  }
}

const [ cookiePath, contestId ] = process.argv.slice(2)

const cookieJar = await jsdom.CookieJar.fromJSON(fs.readFileSync(cookiePath, 'utf8'))

const downloadTasks =
  await jsdom.JSDOM.fromURL(`https://atcoder.jp/contests/${contestId}/tasks`, { cookieJar })
  .then(
    dom => Array.from(
      dom.window.document.querySelectorAll('tr > td:first-child > a'),
      a => (a as HTMLAnchorElement).href
    )
  )
  .then(
    taskURLs => taskURLs.map(
      async (taskURL) => {
        const text =
          await fetch_retry_after(
            500,
            taskURL,
            {
              headers: {
                Cookie: cookieJar.getCookieStringSync('https://atcoder.jp')
              }
            }
          )
          .then(response => response.text())

        const nodes = new jsdom.JSDOM(text).window.document.querySelectorAll('#task-statement > .lang > .lang-ja h3+pre')
        /* see https://img.atcoder.jp/public/8cd1ac6/js/contest.js : 63 */

        const samples: { input: string; output: string }[] = []
        for (let i = 0; i < nodes.length; i += 2) {
          samples.push({
            input: nodes[i]?.textContent ?? '',
            output: nodes[i + 1]?.textContent ?? ''
          })
        }

        const problemId = /_(.+)$/.exec(taskURL)?.[1]
        return [ problemId, samples ]
      }
    )
  )

await Promise.all(downloadTasks)
.then((samples) => {
  console.log(JSON.stringify(Object.fromEntries(samples), null, 2))
})
