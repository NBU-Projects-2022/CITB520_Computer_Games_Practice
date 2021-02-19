fs = require('fs');
const args = process.argv.slice(2);
const offsetAdvance = 4;

function charArray(char, len) {
    return new Array(len).join(char);
}

const map = {
    'track': '[',
    't': '{',
    'name': '{ "name": "',
    'f': '"f": ',
    'fps': '{ "fps": ',
    'i': '"i": "',
    'x': '"x": ',
    'y': '"y": ',
    'kx': '"kx": ',
    'ky': '"ky": ',
    'sx': '"sx": ',
    'sy': '"sy": ',

    //
    '/track': '],',
    '/t': '},',
    '/name': '" },',
    '/fps': ' },',
    '/f': ',',
    '/i': '",',
    '/x': ',',
    '/y': ',',
    '/kx': ',',
    '/ky': ',',
    '/sx': ',',
    '/sy': ',',
}


/**
 * @param {string} str
 */
function nextToken(str, start) {
    let end = start || 0;
    while (str[start] !== '<' && start < str.length) {
        ++start;
    }
    ++start;
    while (str[end] !== '>' && end < str.length) {
        ++end;
    }
    ++end;
    console.log('s', start, str[start], 'e', end, str[end])
    return [
        str.substr(start, end - start - 1),
        start,
        end
    ]
}

/**
 * @param {string} data
 */
function processLine(data, tokenStack) {
    const res = {};
    tokenStack = tokenStack || {};

    console.log('processLine', data);
    let [token, start, end] = nextToken(data, 0);
    console.log([token, start, end])

    let [val, valStart, valEnd] = nextToken(data, end);
    console.log([val, valStart, valEnd])
    res[token] = data.substr(end, valStart - end - 1);

    // console.log(`offset: ${offset}:end offset`);
    console.log(res);
    return res;
}

/**
 * @param {string} data
 */
function processFile(data) {
    const res = {};
    let lastClosingTagNameIndex = 0;
    debugger
    let lastOpenTag = '';
    for (let i = 0; i < data.length; ++i) {
        const isOpenTag = data[i] === '<' && data[i + 1] !== '/';
        const isCloseTag = data[i] === '<' && data[i + 1] === '/';
        console.log(isOpenTag, isCloseTag);

        const parseTagName = (isClosingTag) => {
            const start = i + isClosingTag ? 2 : 1;
            while (data[i] !== '>' && i < data.length) {
                ++i;
            }

            lastClosingTagNameIndex = i + 1;
            return data.substr(start, i - start);
        }

        if (isOpenTag) {
            lastOpenTag = parseTagName(isCloseTag);
        } else if (isCloseTag) {
            if (lastClosingTagNameIndex !== i) {
                console.log(lastOpenTag, lastClosingTagNameIndex, i, data.substr(lastClosingTagNameIndex, i - lastClosingTagNameIndex));
            }
            const tag = parseTagName(isCloseTag);
        } else {
            // value?
        }
    }

    // data.split('\n').slice(0, 5).forEach(line => processLine(line));
    console.log('done');
}

const processJson = (json) => {
    const res = {};
    res.fps = json[0].fps;
    for (let i = 1; i < json.length; ++i) {
        const track = json[i];
        const trackName = track[0].name;
        // if (!track[1].f || track[1].f !== -1) {
        //     console.error('invalid format', trackName, track[1]);
        // }

        res[trackName] = [];
        for (let t = 1; t < track.length; ++t) {
            const f = track[t];
            if (f.i && !f.i.startsWith('IMAGE_REANIM_')) {
                console.log('wtf', f.i);
            }

            res[trackName].push({
                // x: 0,
                // y: 0,
                // sx: 0,
                // sy: 0,
                // kx: 0,
                // ky: 0,
                // f: 1,
                // i: null,
                ...f
            });
        }
    }

    return res;
}

fs.readFile(args[0] || 'Tallnut.reanim', 'utf8',
    /**
     * @param {string} data
     */
    function (err, data) {
        if (err) {
        return console.log(err);
        }

        for (let key in map) {
            data = data.replace(new RegExp(`\\<${key}\\>`, 'g'), map[key]);
        }

        data = data.replace(/,}/g, '}')
            .replace(/},\r\n]/g, '}]')
            .replace(/],\s+$/g, ']');

        let json = JSON.parse(`[ ${data} ]`);
        json = processJson(json);
        // fs.writeFile('test.json', `[ ${data} ]`, 'utf8', () => {});
        fs.writeFile('test.json', JSON.stringify(json, null, 2), 'utf8', () => {});

        let animationsString = Object.keys(json)
            .filter(k => k !== 'fps')
            .map(trackKey => {
                const trackString = json[trackKey]
                    .map(f => {
                        const data = {
                            x: 0,
                            y: 0,
                            sx: 0,
                            sy: 0,
                            kx: 0,
                            ky: 0,
                            f: 1,
                            i: null,
                            ...f
                        };
                        const frameString = [
                                data.x,
                                data.y,
                                data.sx,
                                data.sy,
                                data.kx,
                                data.ky,
                                data.f,
                                data.i ? `"${data.i}"` : 'nullptr'
                            ].join(', ');

                        return `Frame { ${frameString} }`;
                    }).join(',\n');
                return `{
                    "${trackKey}",
                    AnimationTrack {
                        "${trackKey}",
                        std::vector<Frame> {
                            ${trackString}
                        }
                    }
                }`;
            })
            .join(',\n');
        let animationDataString = '';
        animationDataString += `{
            ${json.fps},
            {\n${animationsString}\n}
        }`
        const headerString = `
        #pragma once

        #include <string>
        #include <vector>
        #include <map>

        struct Frame {
            float x, y, sx, sy, kx, ky, f;
            std::string i;
        };

        struct AnimationTrack {
            std::string name;
            std::vector<Frame> frames;
        };

        struct AnimationsContainer {
            int fps;
            std::map<std::string, AnimationTrack> animations;
        };

        const AnimationsContainer test = ${animationDataString};
        `;
        fs.writeFile('test.h', headerString, 'utf8', () => {});
        return;
        data = data.substr(0, 100);
        console.log(data);
        processFile(data);
        // console.log(data);
    });
