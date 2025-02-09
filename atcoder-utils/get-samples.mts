import fs from 'fs/promises'
import jsdom from 'jsdom'

const [ cookiePath, contestId ] = process.argv.slice(2)

const json = await fs.readFile(cookiePath, 'utf8')

const dom = await jsdom.JSDOM.fromURL(
  `https://atcoder.jp/contests/${contestId}/tasks_print`,
  { cookieJar: jsdom.CookieJar.fromJSON(json) }
)

const problems =
  dom.window.document
  .querySelectorAll('.col-sm-12:not(.next-page):not(.alert)')
  .values()
  .map(col_sm_12 => {
    const nodes = col_sm_12.querySelectorAll('#task-statement > .lang > .lang-ja h3+pre')

    const samples: { input: string; output: string }[] = []
    for (let i = 0; i < nodes.length; i += 2) {
      samples.push({
        input: nodes[i].textContent!,
        output: nodes[i + 1].textContent!
      })
    }

    const problemId = /^(.+) -/.exec(col_sm_12.getElementsByClassName('h2')[0]!.textContent!)![1]

    return [ problemId, samples ]
  })

console.log(JSON.stringify(Object.fromEntries(problems), null, 2))
