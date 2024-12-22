type Eid = number;

//CPP binding
interface Binding {
    create_element(tag: string): Eid;
    get_element_by_id(id: string, nodeId: Eid): Eid;
    get_elements_by_tag(tag: string, nodeId: Eid): Eid[];
    get_element_attr(id: Eid, name: string): string | null;
    set_element_attr(id: Eid, name: string, value: string): void;
    append_child(parent_id: Eid, child_id: Eid): void;

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

    body: Element;
    head: Element;

}
interface Element extends Node {
    readonly tagName: string;
    id: string;
    innerHTML: string;
    appendChild(child: Element): void;
    removeChild(child: Element): void;
    setAttribute(name: string, value: string): void;
    getAttribute(name: string): string;
    style: CSSStyleDeclaration;
}
interface CSSStyleDeclaration {
    [key: string]: string;
}
interface Canvas { }
interface Window { }

function Document(): Document {
}