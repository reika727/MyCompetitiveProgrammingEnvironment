import fs from 'fs/promises'
import jsdom from 'jsdom'

const [ cookiePath, contestId ] = process.argv.slice(2)

await fs.readFile(cookiePath, 'utf8')
.then(
  json => jsdom.JSDOM.fromURL(
    `https://atcoder.jp/contests/${contestId}/tasks_print`,
    { cookieJar: jsdom.CookieJar.fromJSON(json) }
  )
)
.then(
  dom => dom.window.document.querySelectorAll('.col-sm-12:not(.next-page):not(.alert)')
)
.then(
  col_sm_12s => col_sm_12s.values().map(
    async col_sm_12 => {
      const nodes = col_sm_12.querySelectorAll('#task-statement > .lang > .lang-ja h3+pre')

      const samples: { input: string; output: string }[] = []
      for (let i = 0; i < nodes.length; i += 2) {
        samples.push({
          input: nodes[i].textContent!,
          output: nodes[i + 1].textContent!
        })
      }

      const problemId = /^(.+) -/.exec(col_sm_12.querySelector('.h2')!.textContent!)![1]

      return [ problemId, samples ]
    }
  )
)
.then(downloadTasks => Promise.all(downloadTasks))
.then(samples =>
  console.log(JSON.stringify(Object.fromEntries(samples), null, 2))
)
