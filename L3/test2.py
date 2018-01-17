import re
def question1c(l):
    return l[5]+l[10]
#
def question2h(l):
    return [e for e in l if e <= 7]
#
def question3c(l):
    return [[ee*2 for ee in e] for e in l if len(e) >=3]
#
def question4c(ref):
    return re.findall(r"\b\w*?[cC]\w*?\b",open(ref).read())
#
def question5e(l):
    return {i:l[i] for i in range(0,len(l)) if l[i]==1 or l[i]==4}
#
def question6b(s):
    return re.findall(r"\d{3,}",s)
#
def question7b(ref):
    d={}
    for l in open(ref).readlines():
        m=re.match(r"<([A-Z]+?)> (.*?) </\1>",l)
        if m !=None:
            d[m.group(1)]=m.group(2)
    return d
#
def question8a(ref):
    return {l.split(",")[4].strip("\n"):(l.split(",")[0],l.split(",")[1]) for l in open(ref).readlines()}
#
def question9b(dico,lat_ref):
    return [ coord[0] for coord in dico.values()if lat_ref<=float(coord[0]) and float(coord[0])<(lat_ref+1)]
#
def question10a(dico,pref):
    l=[nom for nom in dico.keys() if re.match(pref,nom)]
    l.sort()
    return "\t".join(l)
#
