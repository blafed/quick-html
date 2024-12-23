// var log: (msg: string) => void

var __document: Document = Document();
var binding: Binding
var window = Window(__document);

type Eid = ArrayBuffer;


//CPP binding
interface Binding {
    create_element(tag: string): Eid;
    get_element_by_id(id: string, nodeId: Eid): Eid;
    get_elements_by_tag(tag: string, nodeId: Eid): Eid[];
    get_element_attr(id: Eid, name: string): string | null;
    set_element_attr(id: Eid, name: string, vale: string): void;
    append_child(parent_id: Eid, child_id: Eid): void;
    element_tagName(id: Eid): string;

    window_width(): number;
    window_height(): number;

    document_body(): Eid | null;
    document_head(): Eid | null;

    // get_element_id(id: TagId): Element;
    // set_element_id(id: TagId, element: Element): void;
    // get_element_tag(id: TagId): string;
    // set_element_tag(id: TagId, tag: string): void;
    // get_element_inner_html(id: TagId): string;
    // set_element_inner_html(id: TagId, inner_html: string): void;
    // remove_child(parent_id: TagId, child_id: TagId): void;
    // set_attribute(id: TagId, name: string, value: string): void;
    // get_attribute(id: TagId, name: string): string;
    // set_style(id: TagId, name: string, value: string): void;
    // get_style(id: TagId, name: string): string;
}

interface Node {
    getElementById(id: string): Element;
    getElementsByTagName(tag: string): Element[];
    appendChild(child: Element): void;
}
interface Document extends Node {
    createElement(tag: string): Element;
    readonly body: Element;
    readonly head: Element;
}
interface Window {
    readonly innerWidth: number;
    readonly innerHeight: number;
    readonly document: Document;

}
interface Element extends Node {
    readonly _id: Eid;
    readonly tagName: string;
    id: string;
    // innerHTML: string;
    // removeChild(child: Element): void;
    setAttribute(name: string, value: string): void;
    getAttribute(name: string): string;
    // style: CSSStyleDeclaration;
}
interface CSSStyleDeclaration {
    [key: string]: string;
}
interface Canvas { }


function Document(): Document {
    let _head = binding.document_head();
    let _body = binding.document_body();

    let __headObj: Element = Element(_head);
    let __bodyObj: Element = Element(_body);

    return {
        get head() { return __headObj; },
        get body() { return __bodyObj; },
    }
}

function Window(d: Document): Window {
    return {
        get innerWidth() { return binding.window_width(); },
        get innerHeight() { return binding.window_height(); },
        get document() { return d },
    }
}

function Element(eid: Eid): Element {
    return {
        _id: eid,
        get tagName() { return binding.element_tagName(eid); },
        set id(id: string) { binding.set_element_attr(eid, "id", id); },
        get id() { return binding.get_element_attr(eid, "id") ?? ''; },
        // get innerHTML() { return binding.element_innerHTML(eid); },
        // set innerHTML(html: string) { binding.set_element_innerHTML(eid, html); },
        // removeChild(child: Element) { binding.remove_child(eid, child._id); },
        setAttribute(name: string, value: string) { binding.set_element_attr(eid, name, value); },
        getAttribute(name: string) { return binding.get_element_attr(eid, name); },
        appendChild(child: Element) { binding.append_child(eid, child._id); },
        // get style() { return CSSStyleDeclaration(eid); },
    }
}

log("head name " + window.document.body.getAttribute('style'))
