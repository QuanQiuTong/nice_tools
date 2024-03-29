import re

MARKDOWN = {
    'h1': ('\n# ', '\n'),
    'h2': ('\n## ', '\n'),
    'h3': ('\n### ', '\n'),
    'h4': ('\n#### ', '\n'),
    'h5': ('\n##### ', '\n'),
    'h6': ('\n###### ', '\n'),
    'code': ('`', '`'),
    'ul': ('', ''),
    'ol': ('', ''),
    'li': ('- ', ''),
    'blockquote': ('\n> ', '\n'),
    'em': ('**', '**'),
    'strong': ('**', '**'),
    'block_code': ('\n```\n', '\n```\n'),
    'span': ('', ''),
    'p': ('\n', '\n'),
    'p_with_out_class': ('\n', '\n'),
    'inline_p': ('', ''),
    'inline_p_with_out_class': ('', ''),
    'b': ('**', '**'),
    'i': ('*', '*'),
    'del': ('~~', '~~'),
    'hr': ('\n---', '\n\n'),
    'thead': ('\n', '|------\n'),
    'tbody': ('\n', '\n'),
    'td': ('|', ''),
    'th': ('|', ''),
    'tr': ('', '\n')
}

BlOCK_ELEMENTS = {
    'h1': '<h1.*?>(.*?)</h1>',
    'h2': '<h2.*?>(.*?)</h2>',
    'h3': '<h3.*?>(.*?)</h3>',
    'h4': '<h4.*?>(.*?)</h4>',
    'h5': '<h5.*?>(.*?)</h5>',
    'h6': '<h6.*?>(.*?)</h6>',
    'hr': '<hr/>',
    'blockquote': '<blockquote.*?>(.*?)</blockquote>',
    'ul': '<ul.*?>(.*?)</ul>',
    'ol': '<ol.*?>(.*?)</ol>',
    'block_code': '<pre.*?><code.*?>(.*?)</code></pre>',
    'p': '<p\s.*?>(.*?)</p>',
    'p_with_out_class': '<p>(.*?)</p>',
    'thead': '<thead.*?>(.*?)</thead>',
    'tr': '<tr>(.*?)</tr>'
}

INLINE_ELEMENTS = {
    'td': '<td>(.*?)</td>',
    'tr': '<tr>(.*?)</tr>',
    'th': '<th>(.*?)</th>',
    'b': '<b>(.*?)</b>',
    'i': '<i>(.*?)</i>',
    'del': '<del>(.*?)</del>',
    'inline_p': '<p\s.*?>(.*?)</p>',
    'inline_p_with_out_class': '<p>(.*?)</p>',
    'code': '<code.*?>(.*?)</code>',
    'span': '<span.*?>(.*?)</span>',
    'ul': '<ul.*?>(.*?)</ul>',
    'ol': '<ol.*?>(.*?)</ol>',
    'li': '<li.*?>(.*?)</li>',
    'img': '<img.*?src="(.*?)".*?>(.*?)</img>',
    'a': '<a.*?href="(.*?)".*?>(.*?)</a>',
    'em': '<em.*?>(.*?)</em>',
    'strong': '<strong.*?>(.*?)</strong>'
}

DELETE_ELEMENTS = ['<span.*?>', '</span>', '<div.*?>', '</div>']


class Element:
    def __init__(self, start_pos, end_pos, content, tag, is_block=False):
        self.start_pos = start_pos
        self.end_pos = end_pos
        self.content = content
        self._elements = []
        self.is_block = is_block
        self.tag = tag
        self._result = None

        if self.is_block:
            self.parse_inline()

    def __str__(self):
        wrapper = MARKDOWN.get(self.tag)
        self._result = '{}{}{}'.format(wrapper[0], self.content, wrapper[1])
        return self._result

    def parse_inline(self):
        for tag, pattern in INLINE_ELEMENTS.items():

            if tag == 'a':
                self.content = re.sub(pattern, '[\g<2>](\g<1>)', self.content)
            elif tag == 'img':
                self.content = re.sub(pattern, '![\g<2>](\g<1>)', self.content)
            elif self.tag == 'ul' and tag == 'li':
                self.content = re.sub(pattern, '- \g<1>', self.content)
            elif self.tag == 'ol' and tag == 'li':
                self.content = re.sub(pattern, '1. \g<1>', self.content)
            elif self.tag == 'thead' and tag == 'tr':
                self.content = re.sub(pattern, '\g<1>\n',
                                      self.content.replace('\n', ''))
            elif self.tag == 'tr' and tag == 'th':
                self.content = re.sub(
                    pattern, '|\g<1>', self.content.replace('\n', ''))
            elif self.tag == 'tr' and tag == 'td':
                self.content = re.sub(
                    pattern, '|\g<1>', self.content.replace('\n', ''))
            else:
                wrapper = MARKDOWN.get(tag)
                self.content = re.sub(pattern, '{}\g<1>{}'.format(
                    wrapper[0], wrapper[1]), self.content)


class Tomd:
    def __init__(self, html='', options=None):
        self.html = html
        self.options = options
        self._markdown = ''

    def convert(self, html, options=None):
        elements = []
        for tag, pattern in BlOCK_ELEMENTS.items():
            for m in re.finditer(pattern, html, re.I | re.S | re.M):
                element = Element(start_pos=m.start(),
                                  end_pos=m.end(),
                                  content=''.join(m.groups()),
                                  tag=tag,
                                  is_block=True)
                can_append = True
                for e in elements:
                    if e.start_pos < m.start() and e.end_pos > m.end():
                        can_append = False
                    elif e.start_pos > m.start() and e.end_pos < m.end():
                        elements.remove(e)
                if can_append:
                    elements.append(element)

        elements.sort(key=lambda element: element.start_pos)
        self._markdown = ''.join([str(e) for e in elements])

        for index, element in enumerate(DELETE_ELEMENTS):
            self._markdown = re.sub(element, '', self._markdown)
        return self._markdown

    @property
    def markdown(self):
        self.convert(self.html, self.options)
        return self._markdown


_inst = Tomd()
convert = _inst.convert


def main():
    dir = input('please input the directory of the HTML-file.\n>>> ')+'/'
    #dir = 'C:/Users/fqt15/Desktop/The Merchant of Venice/data/'

    with open(dir+'Merchant of Venice_ List of Scenes.html', 'r') as file:
        raw = file.read()

    # （参考：）用下面的方法找频次最高的词
    stat = {}
    for s in ['<p>', '<br>', '<a>']:
        stat.append(s, len(re.findall(s, raw)))

    # 将所有的HTML引用展开，成为对应的文件内容
    refs = re.findall('<a.*?href="./(.*?)".*?>.*?</a>', raw)
    for i in range(len(refs)):
        ref = re.sub('<.*?>', '', convert(open(dir+refs[i], 'r').read()))
        raw = re.sub('<a.*?href=".*?".*?>.*?</a>', ref, raw, 1)

    # 将Act显示为二级标题
    acts = re.findall('Act ([0-9]{1,}), Scene 1:', raw)
    for i in range(len(acts)):
        raw = re.sub('Act [0-9]{1,}, Scene 1:',
                     '## Act '+str(i+1)+'\n', raw, 1)
    raw = re.sub('Act [0-9]{1,}, Scene [0-9]{1,}:', '\n', raw)

    md = convert(raw)

    # 空格和回车的善后工作
    md = re.sub('<br>', '\n', md)
    md = re.sub('\n\n\n\n\n?', '\n\n---\n\n', md)
    md = re.sub('  ', '', md)
    md = re.sub('[\n]{3,}', '\n\n', md)

    # 大标题
    md = '# '+re.findall('<td.*?>(.*?)</td>', md)[0]+md
    md = re.sub('<td.*?>.*?</td>', '', md)

    with open('out3.md', 'w') as file:
        file.write(md)


if __name__ == '__main__':
    main()
